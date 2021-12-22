// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TargetTrace.h"
#include "../Enemy/EnemyAIController.h"
#include "../Enemy/EnemyCharacter.h"
#include "../Player/PrototypeCharacter.h"

UBTTask_TargetTrace::UBTTask_TargetTrace()
{
	NodeName = TEXT("TargetTrace");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_TargetTrace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	//EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = EnemyCharacter->GetEnemyInfo().MoveSpeed;
	UAIBlueprintHelperLibrary::SimpleMoveToActor(AIController, TargetCharacter);
	EnemyCharacter->ChangeAnimType(EEnemyAnimType::Run);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_TargetTrace::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_TargetTrace::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// InProgress로 해두었기 때문에 여기서 안나간다.
	// 그래서 공격거리 안에 들어왔는지를 여기서도 체크하여 들어갔다면
	// Trace를 종료시킨다.
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
	
	const FEnemyInfo& EnemyInfo = EnemyCharacter->GetEnemyInfo();
	FVector MonsterLocation = EnemyCharacter->GetActorLocation();
	const FVector TargetLocation = TargetCharacter->GetActorLocation();

	// 타겟과의 거리를 구한다.
	MonsterLocation.Z = TargetLocation.Z;

	const float Distance = FVector::Distance(MonsterLocation, TargetLocation);
	//PrintViewport(0.01f, FColor::Red, FString::Printf(TEXT("Distance : %f"), Distance));

	if (Distance > EnemyInfo.WatchDistance)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
		EnemyCharacter->ChangeAnimType(EEnemyAnimType::Run);
	}
	else if (Distance > EnemyInfo.AttackDistance)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
		EnemyCharacter->ChangeAnimType(EEnemyAnimType::Walk);
	}
	else if (Distance <= EnemyInfo.AttackDistance)
	{
		AIController->StopMovement();
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		EnemyCharacter->ChangeAnimType(EEnemyAnimType::Idle);
	}

	//if (EEnemyAnimType::Idle == EnemyCharacter->GetAnimType() && 0.f != EnemyCharacter->GetHit())
	//	EnemyCharacter->ChangeAnimType(EEnemyAnimType::Run);
}
