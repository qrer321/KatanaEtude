// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BehaviorTree/BTService.h"
#include "BTService_AttackRandomize.generated.h"

UCLASS()
class KATANAPROJECT_API UBTService_AttackRandomize : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_AttackRandomize();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
