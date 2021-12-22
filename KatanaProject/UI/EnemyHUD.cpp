// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHUD.h"

void UEnemyHUD::NativeConstruct()
{
	Super::NativeConstruct();

	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));

	m_HPBar->SetPercent(1.f);
}

void UEnemyHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
