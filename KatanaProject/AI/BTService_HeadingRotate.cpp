// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_HeadingRotate.h"
#include "../Player/PrototypeCharacter.h"
#include "../Enemy/EnemyCharacter.h"
#include "../Enemy/EnemyAIController.h"

UBTService_HeadingRotate::UBTService_HeadingRotate()
{
	NodeName = TEXT("Heading Rotate");
	Interval = 0.001f;
	RandomDeviation = 0.001f;
}

void UBTService_HeadingRotate::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
		return;

	// AIController는 자신이 빙의되어 있는 Pawn을 가지고 있다
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(AIController->GetPawn());
	if (!EnemyCharacter)
		return;

	APrototypeCharacter* PlayerCharacter = Cast<APrototypeCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (!PlayerCharacter)
		return;

	if (EnemyCharacter->GetAnimType() == EEnemyAnimType::HeavyAttack)
		return;

	const FRotator EnemyRotation = EnemyCharacter->GetActorRotation();
	const FVector EnemyLocation = EnemyCharacter->GetActorLocation();
	const FVector TargetLocation = PlayerCharacter->GetActorLocation();

	const FRotator ToTargetRotation = UKismetMathLibrary::FindLookAtRotation(EnemyLocation, TargetLocation);
	const FRotator HeadingRotation = FMath::RInterpTo(EnemyRotation, ToTargetRotation, DeltaSeconds, 5.f);
	const FRotator HeadingYawRotation = FRotator(0, HeadingRotation.Yaw, 0);
	
	EnemyCharacter->SetActorRotation(HeadingYawRotation);
}
