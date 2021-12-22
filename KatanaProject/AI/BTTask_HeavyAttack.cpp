// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_HeavyAttack.h"
#include "../Enemy/EnemyCharacter.h"
#include "../Enemy/EnemyAIController.h"
#include "../Enemy/EnemyAnimInstance.h"
#include "../Player/PrototypeCharacter.h"

UBTTask_HeavyAttack::UBTTask_HeavyAttack()
{
	NodeName = TEXT("HeavyAttack");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_HeavyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
		return EBTNodeResult::Failed;

	// pController->GetPawn() : AIController는 자신이 빙의되어있는 Pawn을 가지고 있다.
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(AIController->GetPawn());
	if (!EnemyCharacter)
		return EBTNodeResult::Failed;

	APrototypeCharacter* TargetCharacter = Cast<APrototypeCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (!TargetCharacter)
	{
		AIController->StopMovement();
		return EBTNodeResult::Failed;
	}

	AIController->StopMovement();
	EnemyCharacter->ChangeAnimType(EEnemyAnimType::HeavyAttack);
	EnemyCharacter->SetAttackEnd();

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_HeavyAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_HeavyAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		AIController->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// pController->GetPawn() : AIController는 자신이 빙의되어있는 Pawn을 가지고 있다.
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
	if (Hit != 0.f || EnemyCharacter->GetAnimType() == EEnemyAnimType::Defence)
	{
		AIController->StopMovement();
		EnemyCharacter->ResetAttackIndex();
		EnemyCharacter->ChangeAnimType(EEnemyAnimType::Idle);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		UEnemyAnimInstance* EnemyAnimInst = EnemyCharacter->GetAnimInstance();
		if (EnemyAnimInst)
		{
			if (EnemyAnimInst->IsPerilousVisible())
			{
				EnemyAnimInst->SetPerilousVisible(false);
				TargetCharacter->SetPerilousCount(TargetCharacter->GetPerilousCount() - 1);

				if (TargetCharacter->GetPerilousCount() == 0)
					TargetCharacter->SetPerilousVisible(false);
			}
		}
			
		return;
	}

	if (EnemyCharacter->IsCoolTime())
	{
		EnemyCharacter->ChangeAnimType(EEnemyAnimType::Idle);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
