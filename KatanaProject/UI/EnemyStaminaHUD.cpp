// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStaminaHUD.h"

void UEnemyStaminaHUD::NativeConstruct()
{
	Super::NativeConstruct();

	m_StaminaBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("StaminaBar")));

	m_StaminaBar->SetRenderScale(FVector2D(0.f, 1.f));
}

void UEnemyStaminaHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UEnemyStaminaHUD::SetStaminaPercent(float Percent) const
{
	if (Percent < 0.f)
		Percent = 0.f;
	else if (Percent > 1.f)
		Percent = 1.f;
	
	m_StaminaBar->SetRenderScale(FVector2D(1.f - Percent, 1.f));
}
