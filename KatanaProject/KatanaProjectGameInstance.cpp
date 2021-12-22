// Fill out your copyright notice in the Description page of Project Settings.


#include "KatanaProjectGameInstance.h"

UKatanaProjectGameInstance::UKatanaProjectGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> PlayerInfoTableAsset(TEXT("DataTable'/Game/KatanaProject/Player/data/DTPlayerInfo.DTPlayerInfo'"));
	if (PlayerInfoTableAsset.Succeeded())
		m_PlayerInfoTable = PlayerInfoTableAsset.Object;
	
	static ConstructorHelpers::FObjectFinder<UDataTable> EnemyInfoTableAsset(TEXT("DataTable'/Game/KatanaProject/Enemy/Data/DTEnemyInfo.DTEnemyInfo'"));
	if (EnemyInfoTableAsset.Succeeded())
		m_EnemyInfoTable = EnemyInfoTableAsset.Object;
}

UKatanaProjectGameInstance::~UKatanaProjectGameInstance()
{
}

void UKatanaProjectGameInstance::Init()
{
	Super::Init();

	
}

const FPlayerTableInfo* UKatanaProjectGameInstance::FindPlayerInfo(const FString& Name) const
{
	return m_PlayerInfoTable->FindRow<FPlayerTableInfo>(*Name, "");
}

const FEnemyTableInfo* UKatanaProjectGameInstance::FindEnemyInfo(const FString& Name) const
{
	return m_EnemyInfoTable->FindRow<FEnemyTableInfo>(*Name, "");
}
