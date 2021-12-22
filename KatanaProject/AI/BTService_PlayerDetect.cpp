// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerDetect.h"
#include "../Enemy/EnemyCharacter.h"
#include "../Enemy/EnemyAIController.h"
#include "../Player/PrototypeCharacter.h"

UBTService_PlayerDetect::UBTService_PlayerDetect()
{
	NodeName = TEXT("Player Detect");
	Interval = 0.5f;					// 넣은 시간마다 BT 서비스 실행
	// Random Deviation					// Interval 값에 랜덤하게 더함
}

void UBTService_PlayerDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
		return;

	// AIController는 자신이 빙의되어 있는 Pawn을 가지고 있다
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(AIController->GetPawn());
	if (!EnemyCharacter)
		return;

	if (!EnemyCharacter->GetAnimInstance())
		return;

	// 이미 플레이어를 찾았다면 새롭게 찾지 않고
	// 일정 거리 내부일 경우 계속해서 쫓아간다
	APrototypeCharacter* TargetObject = Cast<APrototypeCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (TargetObject)
	{
		const FVector TargetLocation = TargetObject->GetActorLocation();
		const float DistanceDiff = FVector::Distance(TargetLocation, EnemyCharacter->GetActorLocation());

		if (DistanceDiff < 2000.f)
			return;

		// 일정 거리에서 벗어났다면 재탐색을 위해 블랙보드의 값을 초기화
		AIController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
	}
		

	const FEnemyInfo& EnemyInfo = EnemyCharacter->GetEnemyInfo();

	FHitResult HitResult;
	FCollisionQueryParams param(NAME_None, false, EnemyCharacter);
	bool Detect = GetWorld()->SweepSingleByChannel(HitResult, EnemyCharacter->GetActorLocation(), EnemyCharacter->GetActorLocation(),
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel4, FCollisionShape::MakeSphere(EnemyInfo.TraceDistance), param);

	FVector EnemyForwardVector = EnemyCharacter->GetActorForwardVector();
	
	if (Detect)
	{
		FVector vDir = HitResult.ImpactPoint - EnemyCharacter->GetActorLocation();
		vDir.Normalize();

		EnemyForwardVector.Normalize();
		const float Dot = FVector::DotProduct(vDir, EnemyForwardVector);

		float Angle = FMath::Acos(Dot);
		Angle = FMath::RadiansToDegrees(Angle);

		if (Angle > 45.f)
			Detect = false;
	}

#if ENABLE_DRAW_DEBUG
	FColor DrawColor = Detect ? FColor::Red : FColor::Green;

	DrawDebugCone(GetWorld(), EnemyCharacter->GetActorLocation(), EnemyCharacter->GetActorForwardVector(), EnemyInfo.TraceDistance,
		FMath::DegreesToRadians(EnemyInfo.AttackAngle), FMath::DegreesToRadians(EnemyInfo.AttackAngle)
		, 20, DrawColor, false, 0.3f);
	//DrawDebugSphere(GetWorld(), EnemyCharacter->GetActorLocation(), EnemyInfo.TraceDistance, 20, DrawColor, false, 0.3f);
#endif
	
	if (Detect)
	{
		// Player를 찾았다면
		AIController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), HitResult.GetActor());
	}
	//else
	//	AIController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);

	const float Hit = EnemyCharacter->GetHit();
	AIController->GetBlackboardComponent()->SetValueAsFloat(TEXT("Hit"), Hit);
}
