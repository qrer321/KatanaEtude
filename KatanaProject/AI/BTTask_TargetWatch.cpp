// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TargetWatch.h"
#include "../Enemy/EnemyAIController.h"
#include "../Enemy/EnemyCharacter.h"
#include "../Player/PrototypeCharacter.h"

UBTTask_TargetWatch::UBTTask_TargetWatch()
{
	NodeName = TEXT("TargetWatch");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_TargetWatch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
		return EBTNodeResult::Failed;

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(AIController->GetPawn());
	if (!EnemyCharacter)
		return EBTNodeResult::Failed;

	APrototypeCharacter* TargetCharacter = Cast<APrototypeCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (!TargetCharacter)
	{
		EnemyCharacter->ChangeAnimType(EEnemyAnimType::Idle);
		AIController->StopMovement();

		return EBTNodeResult::Failed;
	}

	const int32 SideDir = FMath::RandRange(0, 1);
	if (SideDir == 0)
		EnemyCharacter->ChangeAnimType(EEnemyAnimType::SideL);
	else
		EnemyCharacter->ChangeAnimType(EEnemyAnimType::SideR);

	m_WatchTime = 0.f;
	

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_TargetWatch::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_TargetWatch::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);	// 태스크 종료
		return;
	}

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(AIController->GetPawn());
	if (!EnemyCharacter)
	{
		AIController->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	APrototypeCharacter* TargetCharacter = Cast<APrototypeCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (!TargetCharacter)
	{
		EnemyCharacter->ChangeAnimType(EEnemyAnimType::Idle);
		AIController->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const float Hit = AIController->GetBlackboardComponent()->GetValueAsFloat(TEXT("Hit"));
	if (Hit != 0.f)
	{
		AIController->StopMovement();
		//EnemyCharacter->ChangeAnimType(EEnemyAnimType::Idle);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const FEnemyInfo EnemyInfo = EnemyCharacter->GetEnemyInfo();
	FVector MonsterLocation = EnemyCharacter->GetActorLocation();
	const FVector TargetLocation = TargetCharacter->GetActorLocation();

	// 타겟과의 거리를 구한다.
	MonsterLocation.Z = TargetLocation.Z;

	const float Distance = FVector::Distance(MonsterLocation, TargetLocation);

	if (Distance > EnemyInfo.WatchDistance)
	{
		AIController->StopMovement();
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	m_WatchTime += DeltaSeconds;
	if (m_WatchTime >= EnemyInfo.AttackCoolTime)
	{
		EnemyCharacter->ChangeAnimType(EEnemyAnimType::Walk);
		
		if (Distance <= EnemyInfo.AttackDistance)
		{
			AIController->StopMovement();
			
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			EnemyCharacter->ChangeAnimType(EEnemyAnimType::Idle);
		}
		else
			FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	}
}
