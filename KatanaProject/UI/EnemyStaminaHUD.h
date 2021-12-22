// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "EnemyStaminaHUD.generated.h"

UCLASS()
class KATANAPROJECT_API UEnemyStaminaHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProgressBar* m_StaminaBar;

public:
	void SetStaminaPercent(float Percent) const;
	
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
};
