// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "CharacterHUD.h"
#include "StaminaHUD.h"
#include "EnemyHUD.h"
#include "EnemyStaminaHUD.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

UCLASS()
class KATANAPROJECT_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCharacterHUD* m_CharacterHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaminaHUD* m_StaminaHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UEnemyHUD* m_EnemyHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UEnemyStaminaHUD* m_EnemyStaminaHUD;

public:
	UCharacterHUD* GetCharacterHUD() const { return m_CharacterHUD; }
	UStaminaHUD* GetStaminaHUD() const { return m_StaminaHUD; }

	UEnemyHUD* GetEnemyHUD() const { return m_EnemyHUD; }
	UEnemyStaminaHUD* GetEnemyStaminaHUD() const { return m_EnemyStaminaHUD; }

public:
	void SetStaminaVisibility(bool StaminaVisible) const;
	void SetEnemyHUDVisibility(bool EnemyVisible) const;
	void SetEnemyStaminaVisibility(bool StaminaVisible) const;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
