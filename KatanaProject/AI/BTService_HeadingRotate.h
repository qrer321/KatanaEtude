// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BehaviorTree/BTService.h"
#include "BTService_HeadingRotate.generated.h"

UCLASS()
class KATANAPROJECT_API UBTService_HeadingRotate : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_HeadingRotate();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
