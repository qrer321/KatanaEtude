// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_AttackRandomize.h"
#include "../Enemy/EnemyCharacter.h"
#include "../Enemy/EnemyAIController.h"

UBTService_AttackRandomize::UBTService_AttackRandomize()
{
	NodeName = TEXT("Attack Random Task");
	Interval = 0.5f;
	RandomDeviation = 0.1f;
}

void UBTService_AttackRandomize::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
		return;

	// AIController는 자신이 빙의되어 있는 Pawn을 가지고 있다
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(AIController->GetPawn());
	if (!EnemyCharacter)
		return;

	if (EnemyCharacter->IsCoolTime())
	{
		AIController->GetBlackboardComponent()->SetValueAsInt(TEXT("AttackRandomize"), -1);
		return;
	}
		

	const TArray<FComboAttack> ComboArray = EnemyCharacter->GetComboAttackArray();

	// 콤보 설정
	const int32 ComboIndex = FMath::FRandRange(0, ComboArray.Num() - 1);
	AIController->GetBlackboardComponent()->SetValueAsInt(TEXT("ComboIndex"), ComboIndex);

	const int32 AttackRandomize = FMath::FRandRange(0, 100);
	AIController->GetBlackboardComponent()->SetValueAsInt(TEXT("AttackRandomize"), AttackRandomize);
}
