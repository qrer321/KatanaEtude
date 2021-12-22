// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"

void UMainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	m_CharacterHUD = Cast<UCharacterHUD>(GetWidgetFromName(TEXT("UI_CharacterHUD")));
	m_StaminaHUD = Cast<UStaminaHUD>(GetWidgetFromName(TEXT("UI_StaminaHUD")));
	m_EnemyHUD = Cast<UEnemyHUD>(GetWidgetFromName(TEXT("UI_EnemyHUD")));
	m_EnemyStaminaHUD = Cast<UEnemyStaminaHUD>(GetWidgetFromName(TEXT("UI_EnemyStaminaHUD")));
}

void UMainHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMainHUD::SetStaminaVisibility(bool StaminaVisible) const
{
	if (StaminaVisible)
	{
		if (!m_StaminaHUD->IsVisible())
			m_StaminaHUD->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		if (m_StaminaHUD->IsVisible())
			m_StaminaHUD->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMainHUD::SetEnemyHUDVisibility(bool EnemyVisible) const
{
	if (EnemyVisible)
	{
		if (!m_EnemyHUD->IsVisible())
			m_EnemyHUD->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		if (m_EnemyHUD->IsVisible())
			m_EnemyHUD->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMainHUD::SetEnemyStaminaVisibility(bool StaminaVisible) const
{
	if (StaminaVisible)
	{
		if (!m_EnemyStaminaHUD->IsVisible())
			m_EnemyStaminaHUD->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		if (m_EnemyStaminaHUD->IsVisible())
			m_EnemyStaminaHUD->SetVisibility(ESlateVisibility::Collapsed);
	}
}
