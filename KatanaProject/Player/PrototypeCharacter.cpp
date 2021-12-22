// Copyright Epic Games, Inc. All Rights Reserved.

#include "PrototypeCharacter.h"

#include "PlayerAnimation.h"
#include "../KatanaProjectGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "../Enemy/EnemyCharacter.h"
#include "../UI/MainHUD.h"
#include "../UI/EnemyHUD.h"
#include "../UI/EnemyStaminaHUD.h"

//////////////////////////////////////////////////////////////////////////
// AKatanaProjectCharacter

APrototypeCharacter::APrototypeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	m_PerilousBillBoard = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("Billboard"));
	m_PerilousBillBoard->SetupAttachment(GetCapsuleComponent());
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>	MtrlAsset(TEXT("Material'/Game/KatanaProject/Material/MT_Perilous.MT_Perilous'"));
	if (MtrlAsset.Succeeded())
		m_PerilousBillBoard->AddElement(MtrlAsset.Object, nullptr, false, 32.f, 32.f, nullptr);

	m_PerilousBillBoard->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
	m_PerilousBillBoard->SetVisibility(false);

	static ConstructorHelpers::FClassFinder<ADropletEffect> DropletClass(TEXT("Blueprint'/Game/KatanaProject/Effect/BP_Droplets.BP_Droplets_C'"));
	if (DropletClass.Succeeded())
		m_DropletClass = DropletClass.Class;
	
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	m_FallRecoveryMontage = nullptr;

	m_EquipMontageIndex = 0;
	m_ComboIndex = 0;
	m_AttackIndex = 0;

	m_DeflectTime = 0.f;
	m_PerilousCount = 0;

	m_SPRecover = true;
	m_SPRecoverTime = 0.f;
}

void APrototypeCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 애니메이션 인스턴스 설정
	m_AnimInstance = Cast<UPlayerAnimation>(GetMesh()->GetAnimInstance());
}
//////////////////////////////////////////////////////////////////////////
// Input

void APrototypeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APrototypeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APrototypeCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APrototypeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APrototypeCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("ToggleRun", IE_Pressed, this, &APrototypeCharacter::ToggleRunKey);
	PlayerInputComponent->BindAction("CameraLock", IE_Pressed, this, &APrototypeCharacter::CameraLockKey);
	PlayerInputComponent->BindAction("WeaponEquip", IE_Pressed, this, &APrototypeCharacter::WeaponEquipKey);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APrototypeCharacter::AttackKey);
	//PlayerInputComponent->BindAxis("Attack", this, &APrototypeCharacter::AttackValue);
	
	PlayerInputComponent->BindAction("Deflect", IE_Pressed, this, &APrototypeCharacter::DeflectOnKey);
	PlayerInputComponent->BindAction("Deflect", IE_Released, this, &APrototypeCharacter::DeflectOffKey);
	PlayerInputComponent->BindAxis("Deflect", this, &APrototypeCharacter::DeflectKey);
	//PlayerInputComponent->BindAxis("Sprint", this, &APrototypeCharacter::SprintKey);

	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &APrototypeCharacter::DodgeKey);

	
	PlayerInputComponent->BindAction("ReturnMain", IE_Pressed, this, &APrototypeCharacter::ReturnMainMenu);
}
void APrototypeCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// 캐릭터는 컨트롤러의 방향으로 이동할 것이다
	if (m_AnimInstance)
	{
		if (EPlayerStateType::Defence == m_AnimInstance->GetStateType())
		{
			// Defence 상태가 되었다면 해당 상태가 된 이후로부터의 시간 재기
			m_DeflectTime += DeltaSeconds;
		}
		else
		{
			m_AnimInstance->SetDeflect(false);
		}

		// Rotation이 Actor의 회전을 5.f의 속도로 DT만큼 컨트롤러의 회전 값으로 보간될 것이다
		// 해당 Rotation의 Yaw(Up_Vector인 Z) 축만으로 회전할 것이기에 롤(x)과 피치(y)는 필요하지 않다
		const FRotator Rotation = FMath::RInterpTo(GetActorRotation(), GetControlRotation(), DeltaSeconds, 5.f);
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		if (IsEnemyLocked()) // 캐릭터 헤딩 방향 -> 몬스터
		{
			if (!m_EnemyActor)
				return;
			
			const FVector EnemyLocation = m_EnemyActor->GetActorLocation();
			const FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), EnemyLocation);
			
			const FRotator ToEnemyRotation = FMath::RInterpTo(GetActorRotation(), LookRotation, DeltaSeconds, 5.f);
			const FRotator ToEnemyYawRotation(0, ToEnemyRotation.Yaw, 0);
			
			UCharacterMovementComponent* Movement = GetCharacterMovement();
			if (Movement->bOrientRotationToMovement)
				Movement->bOrientRotationToMovement = false;

			if (m_AnimInstance->RootMotionMode == ERootMotionMode::RootMotionFromMontagesOnly)
				m_AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);

			// 카메라 Lock 중에는 가장 가까운 몬스터 방향으로 Actor가 회전할 수 있도록 설정한다
			SetActorRotation(ToEnemyYawRotation);
		}
		else
		{
			UCharacterMovementComponent* Movement = GetCharacterMovement();
			const UAnimMontage* CurrentMontage = m_AnimInstance->GetCurrentActiveMontage();
			
			if (CurrentMontage)
			{
				const FString MontageName = CurrentMontage->GetName();
				if (-1 != MontageName.Find(TEXT("Attack")))
				{
					// 현재 애니메이션에서 Attack 이라는 문자열이 들어있다면...
					SetActorRotation(YawRotation);
				}
				else if (-1 != MontageName.Find(TEXT("RTI")))
				{
					if (Movement->bOrientRotationToMovement)
						Movement->bOrientRotationToMovement = false;

					if (m_AnimInstance->RootMotionMode == ERootMotionMode::RootMotionFromMontagesOnly)
						m_AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
				}
			}
			else if (m_AnimInstance->GetStateType() == EPlayerStateType::Defence)
			{
				if (Movement->bOrientRotationToMovement)
					Movement->bOrientRotationToMovement = false;
			
				if (m_AnimInstance->RootMotionMode == ERootMotionMode::RootMotionFromMontagesOnly)
					m_AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
			
				SetActorRotation(YawRotation);
			}
			else	// Defence 상태일 경우 루트모션을 위해 모드 전환
			{	
				if (!Movement->bOrientRotationToMovement)
					Movement->bOrientRotationToMovement = true;

				if (m_AnimInstance->RootMotionMode == ERootMotionMode::RootMotionFromEverything)
					m_AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
			}
		}
	}
	
}

void APrototypeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APrototypeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APrototypeCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		if (m_AnimInstance)
		{
			if (m_AnimInstance->GetMovementType() == EMovementType::Walking)
			{
				Value *= .25f;
			}
		}

		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APrototypeCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{	
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		if (m_AnimInstance)
		{
			if (m_AnimInstance->GetMovementType() == EMovementType::Walking)
			{
				Value *= .25f;
			}
		}
		
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APrototypeCharacter::ToggleRunKey()
{
	if (m_AnimInstance)
	{
		switch ((uint8)m_AnimInstance->GetMovementType())
		{
		case EMovementType::Walking:
			m_AnimInstance->ChangeMovementType(EMovementType::Running);
			
			break;
		case EMovementType::Running:
			m_AnimInstance->ChangeMovementType(EMovementType::Walking);
			break;
		}		
	}
	
}

void APrototypeCharacter::CameraLockKey()
{
	// 공중에서의 Lock 기능 막기
	if (!GetMovementComponent()->IsMovingOnGround())
		return;

	// Root모션과 InPlace 모션의 전환에서
	// Root -> InPlace의 애님몽타주가 실행될때는 Blend1D로 움직일 수 가 없다
	// ㄴ 이 문제를 해결하기 위해 동일한 이름의 InPlace 몽타주로 전환
	const UAnimMontage* CurrentMontage = m_AnimInstance->GetCurrentActiveMontage();
	if (CurrentMontage && m_IsEnemyLocked)
	{
		const FString MontageName = CurrentMontage->GetName();
		const float MontagePos = m_AnimInstance->Montage_GetPosition(CurrentMontage);
		
		if (-1 != MontageName.Find(TEXT("WeaponEquip")) ||
			-1 != MontageName.Find(TEXT("WeaponUnEquip")))
		{
			m_AnimInstance->Montage_SetPosition(CurrentMontage, 0.f);
			m_AnimInstance->Montage_Play(m_WeaponEquipInPlaceArray[m_EquipMontageIndex], 1, EMontagePlayReturnType::MontageLength, MontagePos);
		}

		m_EquipMontageIndex = 0;
	}
}

void APrototypeCharacter::WeaponEquipKey()
{
	const UAnimMontage* CurrentMontage = m_AnimInstance->GetCurrentActiveMontage();
	if (CurrentMontage)
	{
		const FString MontageName = CurrentMontage->GetName();
		if (-1 != MontageName.Find(TEXT("Equip")))
		{
			// 현재 애니메이션에서 Equip 이라는 문자열이 들어있다면...
			return;
		}
	}
	
	if (m_AnimInstance->GetWeaponType() == EWeaponType::None)
	{
		int32 EquipRand = 0;
		if (IsEnemyLocked())
		{
			EquipRand = FMath::RandRange(0, m_WeaponEquipRootArray.Num() - 1);
			PlayAnimMontage(m_WeaponEquipRootArray[EquipRand]);
		}
		else
		{
			EquipRand = FMath::RandRange(0, m_WeaponEquipInPlaceArray.Num() - 1);
			PlayAnimMontage(m_WeaponEquipInPlaceArray[EquipRand]);
		}

		m_EquipMontageIndex = EquipRand;
	}
	else if (m_AnimInstance->GetWeaponType() == EWeaponType::Katana)
	{
		int32 EquipRand = 0;
		
		if (IsEnemyLocked())
		{
			EquipRand = FMath::RandRange(0, m_WeaponUnEquipRootArray.Num() - 1);
			PlayAnimMontage(m_WeaponUnEquipRootArray[EquipRand]);
		}
		else
		{
			EquipRand = FMath::RandRange(0, m_WeaponUnEquipInPlaceArray.Num() - 1);
			PlayAnimMontage(m_WeaponUnEquipInPlaceArray[EquipRand]);
		}

		m_EquipMontageIndex = EquipRand;
	}
}

void APrototypeCharacter::AttackKey()
{
	if (m_AnimInstance->GetWeaponType() == EWeaponType::None)
	{
		WeaponEquipKey();
		return;
	}

	if (!m_AnimInstance->CanBeOtherAction())
		return;

	m_AnimInstance->ChangeStateType(EPlayerStateType::Idle);
	
	const UAnimMontage* CurrentMontage = m_AnimInstance->GetCurrentActiveMontage();
	if (CurrentMontage)
	{
		const FString MontageName = CurrentMontage->GetName();
		if (-1 != MontageName.Find(TEXT("Equip")))
		{
			// 현재 애니메이션에서 Equip 이라는 문자열이 들어있다면...
			return;
		}
	}

	Attack();
}

void APrototypeCharacter::AttackValue(float Value)
{
	if (1.f == Value)
	{
		AttackKey();
	}
}

void APrototypeCharacter::DeflectOnKey()
{
	if (m_AnimInstance->GetWeaponType() == EWeaponType::None)
	{
		//WeaponEquipKey();
		return;
	}

	// 회피중이기에 다른 동작을 막는다
	// 만약 다른 동작이 가능한 타이밍이라면 Dodge를 비활성화 해준다
	if (!m_AnimInstance->CanBeOtherAction())
	{
		GetWorldTimerManager().SetTimer(m_StillDeflectTimer, this, &APrototypeCharacter::StillDeflect, 0.1f, true);
		return;
	}
	
	m_DeflectTime = 0.f;
	m_AnimInstance->ChangeStateType(EPlayerStateType::Defence);
	//m_AnimInstance->ChangeMovementType(EMovementType::Walking);
	m_AnimInstance->SetDeflect(true);

	if (1.f == GetInputAxisValue(TEXT("MoveRight")))
		m_AnimInstance->ChangeDefenceDirType(EDefenceDirType::DefenceR);
	else
		m_AnimInstance->ChangeDefenceDirType(EDefenceDirType::DefenceL);
	
	if (m_AnimInstance->GetDefenceDirType() == EDefenceDirType::DefenceL)
	{
		PlayAnimMontage(m_ChangeDefenceArray[0]);
	}
	else if (m_AnimInstance->GetDefenceDirType() == EDefenceDirType::DefenceR)
	{
		PlayAnimMontage(m_ChangeDefenceArray[1]);
	}
}

void APrototypeCharacter::DeflectOffKey()
{
	if (m_AnimInstance->GetWeaponType() == EWeaponType::None ||
		!m_AnimInstance->IsDeflect())
	{
		m_AnimInstance->ChangeStateType(EPlayerStateType::Idle);
		return;
	}
		

	// 현재 애님 몽타주가 Defence일 경우 Timer를 켜서 대기
	const UAnimMontage* CurrentMontage = m_AnimInstance->GetCurrentActiveMontage();
	if (CurrentMontage)
	{
		const FString MontageName = CurrentMontage->GetName();
		if (-1 != MontageName.Find(TEXT("Defense")))
		{
			if (!GetWorldTimerManager().IsTimerActive(m_DeflectTimer))
				GetWorldTimerManager().SetTimer(m_DeflectTimer, this, &APrototypeCharacter::IsDeflect, 0.1f, true);
				
			return;
		}
	}

	m_AnimInstance->SetDeflect(false);
	m_AnimInstance->ChangeStateType(EPlayerStateType::Idle);
	//m_AnimInstance->ChangeMovementType(EMovementType::Running);

	if (m_AnimInstance->GetDefenceDirType() == EDefenceDirType::DefenceL)
		PlayAnimMontage(m_ChangeDefenceArray[2]);
	else if (m_AnimInstance->GetDefenceDirType() == EDefenceDirType::DefenceR)
		PlayAnimMontage(m_ChangeDefenceArray[3]);
}

void APrototypeCharacter::IsDeflect()
{
	const UAnimMontage* CurrentMontage = m_AnimInstance->GetCurrentActiveMontage();
	if (CurrentMontage)
	{
		const FString MontageName = CurrentMontage->GetName();
		if (-1 == MontageName.Find(TEXT("Defense")))
		{
			GetWorldTimerManager().ClearTimer(m_DeflectTimer);
			DeflectOffKey();
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(m_DeflectTimer);
		DeflectOffKey();
	}
}

void APrototypeCharacter::StillDeflect()
{
	if (GetInputAxisValue(TEXT("Deflect")) != 1.f)
	{
		PrintViewport(1.f, FColor::Red, TEXT("Released Deflect"));
		GetWorldTimerManager().ClearTimer(m_StillDeflectTimer);
		return;
	}
	
	if (m_AnimInstance->CanBeOtherAction())
	{
		GetWorldTimerManager().ClearTimer(m_StillDeflectTimer);
		DeflectOnKey();
	}
}

void APrototypeCharacter::SetPerilousVisible(bool Visible) const
{
	if (Visible)
		GetWorldSettings()->SetTimeDilation(0.5f);
	else
		GetWorldSettings()->SetTimeDilation(1.f);
	
	m_PerilousBillBoard->SetVisibility(Visible);
}

void APrototypeCharacter::DodgeKey()
{
	if (m_AnimInstance->GetWeaponType() == EWeaponType::None)
		return;

	// 회피중이기에 다른 동작을 막는다
	// 만약 다른 동작이 가능한 타이밍이라면 Dodge를 비활성화 해준다
	if (!m_AnimInstance->CanBeOtherAction())
		return;

	m_AnimInstance->ChangeStateType(EPlayerStateType::Idle);
	
	// Dodge키 누르면 실행되고 있는 모든 애니메이션 All Stop
	m_AnimInstance->StopAllMontages(0.4f);

	// HeavyAttack을 회피하는 경우
	if (m_PerilousBillBoard->IsVisible())
		GetWorldSettings()->SetTimeDilation(1.f);

	
	if (IsEnemyLocked())
		Avoid();
	else
		Slide();
}

void APrototypeCharacter::ReturnMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("StartLevel"));
}

const FVector& APrototypeCharacter::GetDesiredVelocity()
{
	const float ForwardValue = GetInputAxisValue(TEXT("MoveForward"));
	const float RightValue = GetInputAxisValue(TEXT("MoveRight"));

	// find out which way is forward
	const FRotator Rotation = GetActorRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);	// get forward vector 
	const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);	// get right vector
	
	// ForwardVector와 RightVector를 합한 값을 Normalize 하여 return
    // 두 방향키의 입력값으로 8방향의 방향벡터가 리턴됨
	FVector DesiredDirection = (ForwardDir * ForwardValue) + (RightDir * RightValue);
	DesiredDirection.Normalize();

	m_DesiredDirection = DesiredDirection;
	
	return m_DesiredDirection;
}

void APrototypeCharacter::SetEnemyActor(AEnemyCharacter* EnemyCharacter)
{
	if (!EnemyCharacter)
		return;
	
	// Enemy Actor 설정
	m_EnemyActor = EnemyCharacter;
	m_IsEnemyLocked = true;

	// EnemyActor가 등록되었다면 관련된 Enemy HUD 활성화
	AKatanaProjectGameMode* GameMode = Cast<AKatanaProjectGameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(GameMode))
	{
		const UMainHUD* MainHUD = GameMode->GetMainHUD();
		if (IsValid(MainHUD))
		{
			MainHUD->SetEnemyHUDVisibility(true);

			// Enemy Lock UnLock 이후 다시 Lock 했을 때
			// 체간이 초기화 값과 동일하지 않은 경우 == 체간이 쌓여있다
			// -> 바로 스테미나 창이 보여질 수 있도록 설정
			if (EnemyCharacter->GetEnemyInfo().SP != EnemyCharacter->GetEnemyInfo().SPMax)
				MainHUD->SetEnemyStaminaVisibility(true);

			const UEnemyHUD* EnemyHUD = MainHUD->GetEnemyHUD();
			const UEnemyStaminaHUD* EnemyStaminaHUD = MainHUD->GetEnemyStaminaHUD();

			if (IsValid(EnemyHUD) && IsValid(EnemyStaminaHUD))
			{
				EnemyHUD->SetHPPercent(EnemyCharacter->GetEnemyInfo().HP / (float)EnemyCharacter->GetEnemyInfo().HPMax);
				EnemyStaminaHUD->SetStaminaPercent(EnemyCharacter->GetEnemyInfo().SP / (float)EnemyCharacter->GetEnemyInfo().SPMax);
			}
		}
	}
}

void APrototypeCharacter::ResetEnemyActor()
{
	m_EnemyActor = nullptr;
	m_IsEnemyLocked = false;

	AKatanaProjectGameMode* GameMode = Cast<AKatanaProjectGameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(GameMode))
	{
		const UMainHUD* MainHUD = GameMode->GetMainHUD();
		if (IsValid(MainHUD))
		{
			MainHUD->SetEnemyHUDVisibility(false);
			MainHUD->SetEnemyStaminaVisibility(false);
		}
	}
}

void APrototypeCharacter::ActionDismemberToVictim()
{
	// Execute 당하고 있는 Enemy에게 신체결손 함수 호출
	if (m_EnemyActor)
		m_EnemyActor->Dismember();
}