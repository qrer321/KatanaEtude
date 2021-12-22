// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "UI/StartMenuHUD.h"
#include "GameFramework/GameModeBase.h"
#include "KatanaStartGameMode.generated.h"

UCLASS()
class KATANAPROJECT_API AKatanaStartGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AKatanaStartGameMode();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UStartMenuHUD> m_StartHUDClass;

	UPROPERTY()
	UStartMenuHUD* m_StartHUD;

public:
	UStartMenuHUD* GetStartHUD() { return m_StartHUD; }

public:
	virtual void BeginPlay() override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
};
