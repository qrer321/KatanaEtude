// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameInfo.h"
#include "UI/MainHUD.h"
#include "AssetManager/AssetPathMain.h"
#include "GameFramework/GameModeBase.h"
#include "KatanaProjectGameMode.generated.h"

UCLASS(minimalapi)
class AKatanaProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AKatanaProjectGameMode();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMainHUD> m_MainHUDClass;

	UPROPERTY()
	UMainHUD* m_pMainHUD;
	
	// TArray<TSubclassOf<APawn>> m_PlayerClassArray;
	
	UPROPERTY()
	UAssetPathMain* m_pMainAssetPath;

public:
	UMainHUD* GetMainHUD() { return m_pMainHUD; }

	virtual void Tick(float DeltaTime) override;

public:
	virtual void BeginPlay() override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
};