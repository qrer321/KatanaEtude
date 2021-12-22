// Copyright Epic Games, Inc. All Rights Reserved.

#include "KatanaProjectGameMode.h"
#include "Player/PrototypeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AKatanaProjectGameMode::AKatanaProjectGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UMainHUD> MainHudClass(TEXT("WidgetBlueprint'/Game/KatanaProject/UI/UI_MainHUD.UI_MainHUD_C'"));
	if (MainHudClass.Succeeded())
		m_MainHUDClass = MainHudClass.Class;
	
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/KatanaProject/Player/BPPrototypeCharacter.BPPrototypeCharacter_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AKatanaProjectGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKatanaProjectGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(m_MainHUDClass))
	{
		m_pMainHUD = Cast<UMainHUD>(CreateWidget(GetWorld(), m_MainHUDClass));
	
		// 여러개의 위젯이 존재할 경우 ZOrder를 이용하여 나중에 그려질 위젯을 정해줄 수 있다.
		if (m_pMainHUD)
			m_pMainHUD->AddToViewport();
	}
	//
	// APlayerController* pController = GetWorld()->GetFirstPlayerController();
	//
	// //FInputModeUIOnly FMode;
	// // FInputModeGameOnly : Default
	// FInputModeGameAndUI FMode;
	//
	// pController->SetInputMode(FMode);
	// pController->bShowMouseCursor = true;	// 마우스 커서 출력

	m_pMainAssetPath = UAssetPathMain::StaticClass()->GetDefaultObject<UAssetPathMain>();
	m_pMainAssetPath->ConvertPath();
}

void AKatanaProjectGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}
