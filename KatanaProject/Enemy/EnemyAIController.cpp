// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

AEnemyAIController::AEnemyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> AITreeAsset(TEXT("BehaviorTree'/Game/KatanaProject/Enemy/BTEnemy.BTEnemy'"));
	if (AITreeAsset.Succeeded())
		m_AITree = AITreeAsset.Object;
	
	static ConstructorHelpers::FObjectFinder<UBlackboardData> AIBlackboardAsset(TEXT("BlackboardData'/Game/KatanaProject/Enemy/BBEnemy.BBEnemy'"));
	if (AIBlackboardAsset.Succeeded())
		m_AIBlackboard = AIBlackboardAsset.Object;
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(m_AIBlackboard, Blackboard))
	{
		if (!RunBehaviorTree(m_AITree))
		{
			LOG(TEXT("Enemy BehaviorTree Error!!"));
		}
	}
}

void AEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
