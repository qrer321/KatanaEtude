// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckDistance.h"
#include "../Player/PrototypeCharacter.h"
#include "../Enemy/EnemyCharacter.h"
#include "../Enemy/EnemyAIController.h"

UBTDecorator_CheckDistance::UBTDecorator_CheckDistance()
{
	NodeName = TEXT("CheckDistance");
}

bool UBTDecorator_CheckDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool Result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// 조건이 달성되었는지 확인하는 함수
	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
		return false;

	// BlackBoard에 저장된 Target 정보를 얻어온다
	APrototypeCharacter* PlayerCharacter = Cast<APrototypeCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (!PlayerCharacter)
		return false;

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(AIController->GetPawn());
	if (!EnemyCharacter)
		return false;

	const FEnemyInfo& EnemyInfo = EnemyCharacter->GetEnemyInfo();
	FVector EnemyLocation = EnemyCharacter->GetActorLocation();
	FVector TargetLocation = PlayerCharacter->GetActorLocation();

	// 높이 차이 제거
	EnemyLocation.Z = TargetLocation.Z;

	const float Distance = FVector::Distance(EnemyLocation, TargetLocation);
	float CheckDistance = 0;

	switch (m_CheckType)
	{
	case ECheckDistanceType::Trace:
		CheckDistance = EnemyInfo.TraceDistance;
		break;
	case ECheckDistanceType::Watch:
		CheckDistance = EnemyInfo.WatchDistance;
		break;
	case ECheckDistanceType::Attack:
		CheckDistance = EnemyInfo.AttackDistance;
		break;
	}

	return Distance <= CheckDistance;
}
