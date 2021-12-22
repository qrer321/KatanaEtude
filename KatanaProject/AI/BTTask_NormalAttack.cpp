// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_NormalAttack.h"
#include "../Enemy/EnemyCharacter.h"
#include "../Enemy/EnemyAIController.h"
#include "../Player/PrototypeCharacter.h"

UBTTask_NormalAttack::UBTTask_NormalAttack()
{
	NodeName = TEXT("NormalAttack");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_NormalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	EnemyCharacter->ChangeAnimType(EEnemyAnimType::Attack);
	EnemyCharacter->SetAttackEnd();

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_NormalAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_NormalAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	// const FEnemyInfo& EnemyInfo = EnemyCharacter->GetEnemyInfo();
	// FVector EnemyLocation = EnemyCharacter->GetActorLocation();
	// const FVector TargetLocation = TargetCharacter->GetActorLocation();
	//
	// if (EnemyInfo.HP <= 0)
	// {
	// 	EnemyCharacter->ChangeAnimType(EEnemyAnimType::Death);
	// 	
	// 	AIController->StopMovement();
	// 	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	// 	return;
	// }
	//
	// // 높이 차이 제거
	// EnemyLocation.Z = TargetLocation.Z;
	//
	// const float DiffDistance = FVector::Distance(EnemyLocation, TargetLocation);
	//
	// if (DiffDistance > EnemyInfo.AttackDistance)
	// {
	// 	EnemyCharacter->ResetAttackIndex();
	// 	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	// 	return;
	// }

	const float Hit = AIController->GetBlackboardComponent()->GetValueAsFloat(TEXT("Hit"));
	if (Hit != 0.f)
	{
		AIController->StopMovement();
		EnemyCharacter->ResetAttackIndex();
		EnemyCharacter->ChangeAnimType(EEnemyAnimType::Idle);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	if (!EnemyCharacter->IsCoolTime() && EEnemyAnimType::Idle == EnemyCharacter->GetAnimType())
	{
		// 쿨타임 중이 아닌데 Attack 애님이 아니라면 초기화해준다
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else if (EnemyCharacter->IsCoolTime())
	{
		EnemyCharacter->ChangeAnimType(EEnemyAnimType::Idle);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
		
	
	// if (EnemyCharacter->GetAttackEnd())
	// {
	// 	if (DiffDistance > EnemyInfo.AttackDistance)
	// 	{
	// 		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	// 	}
	// 	else
	// 	{
	// 		FVector vDir = TargetLocation - EnemyLocation;
	// 		vDir.Normalize();
	//
	// 		EnemyCharacter->SetActorRotation(FRotator(0.f, vDir.Rotation().Yaw, 0.f));
	// 	}
	//
	// 	EnemyCharacter->SetAttackEnd(false);
	// 	return;
	// }
}
