// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimation.h"

#include "MineProtoCharacter.h"
#include "../Weapon/Weapon.h"
#include "../Weapon/Katana.h"
#include "../Effect/NormalEffect.h"

UPlayerAnimation::UPlayerAnimation()
{
	m_SpeedFwd = 0.f;
	m_SpeedSide = 0.f;

	m_AnimType = EPlayerAnimType::Ground;
	m_StateType = EPlayerStateType::Idle;
	m_MovementType = EMovementType::Running;
	m_WeaponType = EWeaponType::None;
	m_DefenceDirType = EDefenceDirType::DefenceL;

	m_IsInAir = false;
	m_CanOtherAction = true;

	m_RunStartAnim = CreateDefaultSubobject<UAnimSequenceBase>(TEXT("AnimSequence'/Game/GhostSamurai_Bundle/GhostSamurai/Katana/Common/Root/GhostSamurai_Common_Run_Start_Root.GhostSamurai_Common_Run_Start_Root'"));
	m_RunStopAnim = CreateDefaultSubobject<UAnimSequenceBase>(TEXT("AnimSequence'/Game/GhostSamurai_Bundle/GhostSamurai/Katana/Common/Root/GhostSamurai_Common_Run_End_Root.GhostSamurai_Common_Run_End_Root'"));
	m_WalkStartAnim = CreateDefaultSubobject<UAnimSequenceBase>(TEXT("AnimSequence'/Game/GhostSamurai_Bundle/GhostSamurai/Katana/Common/Root/GhostSamurai_Common_Walk_Start_Root.GhostSamurai_Common_Walk_Start_Root'"));
	m_WalkStopAnim = CreateDefaultSubobject<UAnimSequenceBase>(TEXT("AnimSequence'/Game/GhostSamurai_Bundle/GhostSamurai/Katana/Common/Root/GhostSamurai_Common_Walk_End_Root.GhostSamurai_Common_Walk_End_Root'"));

	m_IKInterpSpeed = 15.f;
}

bool UPlayerAnimation::IsEnemyLocked() const
{
	AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
	if (Player)
	{
		if (Player->IsEnemyLocked())
			return true;
	}

	return false;
}

void UPlayerAnimation::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPlayerAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
	if (Player)
	{
		FootPlacement(DeltaSeconds);
		
		m_ForwardAxisValue = Player->GetInputAxisValue(TEXT("MoveForward"));
		m_SideAxisValue = Player->GetInputAxisValue(TEXT("MoveRight"));

		if (m_ForwardAxisValue == 0 && m_SideAxisValue == 0)
			m_BlendValue = 0.f;
		else
			m_BlendValue = 0.4f;
		
		UCharacterMovementComponent* Movement = Player->GetCharacterMovement();
		AController* PlayerController = Player->Controller;
		if (Movement && PlayerController)
		{
			// 적에게 카메라가 고정 되었는지 체크
			if (Player->IsEnemyLocked() || m_StateType == EPlayerStateType::Defence)
			{
				// 현재 방향키로 입력된 전방벡터에 Forward와 Right Direction을 사영하면
				// 전방벡터에 따른 스칼라값이 도출된다
				float Forward = FVector::DotProduct(Player->GetDesiredVelocity(), Player->GetActorForwardVector());
				float Side = FVector::DotProduct(Player->GetDesiredVelocity(), Player->GetActorRightVector());

				float WalkSpeed = 1.f;
				if (m_MovementType == EMovementType::Walking)
					WalkSpeed = 0.25f;

				const float ForwardValue = abs(m_ForwardAxisValue);
				const float SideValue = abs(m_SideAxisValue);
				
				Forward *= Movement->MaxWalkSpeed * ForwardValue * WalkSpeed; 
				Side *= Movement->MaxWalkSpeed * SideValue * WalkSpeed;
			
				// 부드러운 애니메이션 전환을 위한 선형보간 처리
				// 입력값이 없을 때는 빠른 전환이 필요하다고 느껴져서 알파값 변수화
				float Alpha = 0.04f;
				//if (ForwardValue == 0.f && SideValue == 0.f)
				//	Alpha = 0.3f;
				
				m_SpeedFwd = FMath::Lerp(m_SpeedFwd, Forward, Alpha);
				m_SpeedSide = FMath::Lerp(m_SpeedSide, Side, Alpha);

				//PrintViewport(0.01f, FColor::Red, FString::Printf(TEXT("Alpha : %f"), Alpha));
			}
			else
			{				
				// 카메라 고정 상태가 아니라면 Free Moving 상태로
				m_SpeedFwd = FMath::Lerp(m_SpeedFwd, Movement->Velocity.Size(), 0.04f);
				m_SpeedSide = FMath::Lerp(m_SpeedSide, 0.f, 0.04f);

				//StartAndStopAnim();
			}

			m_IsInAir = Movement->IsFalling();
			if (m_IsInAir)
				m_AnimType = EPlayerAnimType::Fall;
		}
	}



	//PrintViewport(0.01f, FColor::Red, FString::Printf(TEXT("Forward : %f"), m_SpeedFwd));
	//PrintViewport(0.01f, FColor::Red, FString::Printf(TEXT("Side : %f"), m_SpeedSide));
}

bool UPlayerAnimation::CanBeOtherAction()
{
	if (m_CanOtherAction)
	{
		AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
		if (Player)
			Player->SetDodge(false);

		return m_CanOtherAction;
	}

	// const UAnimMontage* CurrentMontage = GetCurrentActiveMontage();
	// if (CurrentMontage)
	// {
	// 	const FString MontageName = CurrentMontage->GetName();
	// 	if (-1 != MontageName.Find(TEXT("Avoid")) ||
	// 		-1 != MontageName.Find(TEXT("Dodge")) ||
	// 		-1 != MontageName.Find(TEXT("Slide")))
	// 	{
	// 		m_CanOtherAction = false;
	// 		return m_CanOtherAction;
	// 	}
	// }
	
	return m_CanOtherAction;
}

void UPlayerAnimation::FootPlacement(float DeltaSecond)
{
	AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
	if (!Player)
		return;

	const bool IsFalling = Player->GetCharacterMovement()->IsFalling();
	if (!IsFalling)
	{
		bool RDisBlock = false;
		bool LDisBlock = false;
		float RDis = 0.f;
		float LDis = 0.f;
		float LongestDistance = 0.f;
		
		Dis(TEXT("foot_r"), RDisBlock, RDis);
		Dis(TEXT("foot_l"), LDisBlock, LDis);

		if (RDisBlock || LDisBlock)
		{
			if (LDis >= RDis)
				LongestDistance = LDis;
			else
				LongestDistance = RDis;
		
			const float InterpolationTarget = (LongestDistance - 101.f) * -1.f;
			m_Displacement = UKismetMathLibrary::FInterpTo(m_Displacement, InterpolationTarget, DeltaSecond,
			                                               m_IKInterpSpeed);
		
			bool RTraceBlock = false;
			bool LTraceBlock = false;
			float RTraceDistance = 1000.f;
			float LTraceDistance = 1000.f;
			FVector RNormal = FVector::ZeroVector;
			FVector LNormal = FVector::ZeroVector;
			
			FootLineTrace(TEXT("foot_r"), RTraceBlock, RTraceDistance, RNormal);
			FootLineTrace(TEXT("foot_l"), LTraceBlock, LTraceDistance, LNormal);

			// PrintViewport(0.01f, FColor::Yellow, FString::Printf(TEXT("RDistance : %f"), RTraceDistance));
			// PrintViewport(0.01f, FColor::Yellow, FString::Printf(TEXT("LDistance : %f"), LTraceDistance));
		
			const FRotator RFootRotation = FRotator(UKismetMathLibrary::DegAtan2(RNormal.X, RNormal.Z) * -1.f,
			                                        0.f,
			                                        UKismetMathLibrary::DegAtan2(RNormal.Y, RNormal.Z));
			const FRotator LFootRotation = FRotator(UKismetMathLibrary::DegAtan2(LNormal.X, LNormal.Z) * -1.f,
			                                        0.f,
			                                        UKismetMathLibrary::DegAtan2(LNormal.Y, LNormal.Z));
			
			m_RFootRotation = UKismetMathLibrary::RInterpTo(m_RFootRotation, RFootRotation, DeltaSecond,
			                                                m_IKInterpSpeed);
			m_LFootRotation = UKismetMathLibrary::RInterpTo(m_LFootRotation, LFootRotation, DeltaSecond,
															m_IKInterpSpeed);
			
			RTraceDistance = (RTraceDistance - 100.f) / -35.f;
			LTraceDistance = (LTraceDistance - 100.f) / -35.f;

			// RTraceDistance = UKismetMathLibrary::Clamp(RTraceDistance, 0, 1);
			// LTraceDistance = UKismetMathLibrary::Clamp(LTraceDistance, 0, 1);
			
			m_RIK = UKismetMathLibrary::FInterpTo(m_RIK, RTraceDistance, DeltaSecond, m_IKInterpSpeed);
			m_LIK = UKismetMathLibrary::FInterpTo(m_LIK, LTraceDistance, DeltaSecond, m_IKInterpSpeed);

			// PrintViewport(0.01f, FColor::Yellow, FString::Printf(TEXT("RIK : %f"), m_RIK));
			// PrintViewport(0.01f, FColor::Yellow, FString::Printf(TEXT("LIK : %f"), m_LIK));
		}
	}
}

void UPlayerAnimation::Dis(FName SocketName, bool& BlockingHit, float& Distance) const
{
	AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
	if (!Player)
		return;

	const USkeletalMeshComponent* PlayerMesh = Player->GetMesh();
	
	const FVector SocketLocation = PlayerMesh->GetSocketLocation(SocketName);
	FVector MeshWorldLocation = PlayerMesh->GetComponentLocation();
	MeshWorldLocation.Z += 101.f;

	const FVector TraceStartLocation =  FVector(SocketLocation.X, SocketLocation.Y, MeshWorldLocation.Z);
	const FVector TraceEndLocation = FVector(SocketLocation.X, SocketLocation.Y, MeshWorldLocation.Z - 151.f);
	
	FHitResult result;
	GetWorld()->LineTraceSingleByChannel(result, TraceStartLocation, TraceEndLocation, ECC_GameTraceChannel5);
	
	BlockingHit = result.bBlockingHit;
	Distance = result.Distance;
}

void UPlayerAnimation::FootLineTrace(FName SocketName, bool& BlockingHit, float& Distance, FVector& Normal) const
{
	AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
	if (!Player)
		return;

	const USkeletalMeshComponent* PlayerMesh = Player->GetMesh();
	
	const FVector SocketLocation = PlayerMesh->GetSocketLocation(SocketName);
	const FVector RootLocation = PlayerMesh->GetSocketLocation(TEXT("root"));

	FVector TraceStartLocation = FVector(SocketLocation.X, SocketLocation.Y, RootLocation.Z + 100.f);
	FVector TraceEndLocation = FVector(SocketLocation.X, SocketLocation.Y, RootLocation.Z - 100.f);
	
	FHitResult result;
	FCollisionQueryParams param(NAME_None, false, Player);
	bool Hit = GetWorld()->LineTraceSingleByChannel(result, TraceStartLocation, TraceEndLocation, ECC_GameTraceChannel5);

	if (Hit)
	{
		BlockingHit = result.bBlockingHit;
		Distance = result.Distance;
		Normal = result.Normal;
		
// #if ENABLE_DRAW_DEBUG
// 		FColor DrawColor = Hit ? FColor::Red : FColor::Green;
// 		DrawDebugLine(GetWorld(), TraceStartLocation, TraceEndLocation, DrawColor, false, 0.1f, 0, 1);
// #endif
	}
}


void UPlayerAnimation::StartAndStopAnim()
{
	if (m_SpeedFwd > 151.f)
	{
		// 전방 방향으로의 속도가 150보다 크다면 -> 뛰는 애니메이션으로의 전환이 필요
	}
	else
	{
		// Velocity가 150보다 작다면 -> 멈추는 애니메이션으로의 전환이 필요
		PlaySlotAnimationAsDynamicMontage(m_RunStopAnim, TEXT("GroundMovement"));
	}

	//PlaySlotAnimation();
}

float UPlayerAnimation::GetForwardAxisValue()
{
	AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
	if (Player)
	{
		return Player->GetInputAxisValue(TEXT("MoveForward"));
	}

	return 0.f;
}

float UPlayerAnimation::GetSideAxisValue()
{
	AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
	if (Player)
	{
		return Player->GetInputAxisValue(TEXT("MoveRight"));
	}

	return 0.f;
}

void UPlayerAnimation::AnimNotify_JumpStart()
{
}

void UPlayerAnimation::AnimNotify_JumpEnd()
{
	//m_AnimType = EPlayerAnimType::Fall;
}

void UPlayerAnimation::AnimNotify_LandEnd()
{
	m_AnimType = EPlayerAnimType::Ground;

	m_FallRecoveryAdditive = 0.4f;
	AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
	if (Player)
		Player->FallRecovery();
}

void UPlayerAnimation::AnimNotify_WeaponEquip()
{
	// WeaponEquip Notify 활성화 시...

	// AllInKatana 렌더링 비활성화
	// Katana / Sheath 렌더링 활성화
	AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
	if (Player)
	{
		AWeapon* AllKatana = Player->GetAllKatana();
		AllKatana->SetWeaponOutput(false);

		AKatana* Katana = Player->GetKatana();
		AWeapon* Sheath = Player->GetSheath();

		Katana->SetWeaponOutput(true);
		Sheath->SetWeaponOutput(true);

		m_WeaponType = EWeaponType::Katana;
	}
}

void UPlayerAnimation::AnimNotify_WeaponUnEquip()
{
	// AllInKatana 렌더링 활성화
	AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
	if (Player)
	{
		AWeapon* AllKatana = Player->GetAllKatana();
		AllKatana->SetWeaponOutput(true);

		//AWeapon* Katana = Player->GetKatana();
		AWeapon* Sheath = Player->GetSheath();

		//Katana->SetWeaponOutput(false);
		Sheath->SetWeaponOutput(false);

		m_WeaponType = EWeaponType::None;
	}
}

void UPlayerAnimation::AnimNotify_CheckNextCombo() const
{
	AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
	if (Player)
		Player->DoAttackCombo();
}

void UPlayerAnimation::AnimNotify_ComboCheck() const
{
	AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
	if (Player)
		Player->ComboCheck();
}

void UPlayerAnimation::AnimNotify_OnCollStart_Attack()
{
	AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
	if (Player)
	{
		Player->BeginColl();
	}
}

void UPlayerAnimation::AnimNotify_OnCollEnd_Attack()
{
	AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
	if (Player)
	{
		Player->EndColl();
	}
}

void UPlayerAnimation::AnimNotify_IsMove()
{
	AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
	if (Player)
	{
		if (m_ForwardAxisValue || m_SideAxisValue)
		{
			// 입력 값이 존재한다면
			//PrintViewport(1.f, FColor::Red, TEXT("IsMove"));
			const UAnimMontage* CurrentMontage = GetCurrentActiveMontage();
			Montage_Stop(0.3f, CurrentMontage);
		}
	}
}

void UPlayerAnimation::AnimNotify_HitEnd()
{
	//m_Hit = false;

	AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
	if (Player)
		Player->AttackReset();
}

void UPlayerAnimation::AnimNotify_ChangeType()
{
	switch (m_DefenceDirType)
	{
	case EDefenceDirType::DefenceL:
		m_DefenceDirType = EDefenceDirType::DefenceR;
		break;
	case EDefenceDirType::DefenceR:
		m_DefenceDirType = EDefenceDirType::DefenceL;
		break;
	}
}

void UPlayerAnimation::AnimNotify_Dodge_Start()
{
	m_CanOtherAction = false;

	PrintViewport(1.f, FColor::Red, TEXT("Dodge Start"));
	
	AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
	if (Player)
		Player->SetDodge(true);
}

void UPlayerAnimation::AnimNotify_CanBeOtherAction()
{
	m_CanOtherAction = true;
}

void UPlayerAnimation::AnimNotify_Dodge_End()
{
	m_CanOtherAction = true;
	
	AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
	if (Player)
		Player->SetDodge(false);
}

void UPlayerAnimation::AnimNotify_DropletParticle()
{
	AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
	if (Player)
	{
		const TSubclassOf<ADropletEffect> DropletClass = Player->GetDropletClass();
		if (IsValid(DropletClass))
		{
			const FRotator PlayerRotation = Player->GetActorRotation();
			const FRotator PlayerYawRotation = FRotator(0.f, PlayerRotation.Yaw, 0.f);
			const FVector DropletLocation = Player->GetMesh()->GetSocketLocation(TEXT("DeflectParticle"));
			
			FTransform EffectTrans;
			EffectTrans.SetLocation(DropletLocation);
			EffectTrans.SetRotation(PlayerYawRotation.Quaternion());
			EffectTrans.SetScale3D(FVector(1.f, 1.f, 1.f));
			
			ADropletEffect* DropletEffect = GetWorld()->SpawnActorDeferred<ADropletEffect>(DropletClass, EffectTrans);
			DropletEffect->SetDropletName(EParticleSystemName::Exit_Wound_Shotgun);
			DropletEffect->SetDecalTimeout(20.f);
			DropletEffect->FinishSpawning(EffectTrans);
		}
	}
}

void UPlayerAnimation::AnimNotify_Dismember()
{
	// Dismemberment, 신체결손 기능
	AMineProtoCharacter* Player = Cast<AMineProtoCharacter>(TryGetPawnOwner());
	if (Player)
		Player->ActionDismemberToVictim();
}

