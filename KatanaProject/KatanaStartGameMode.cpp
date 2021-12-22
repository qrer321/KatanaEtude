// Fill out your copyright notice in the Description page of Project Settings.


#include "KatanaStartGameMode.h"

AKatanaStartGameMode::AKatanaStartGameMode()
{
	ConstructorHelpers::FClassFinder<UStartMenuHUD> StartHUDClass(TEXT("WidgetBlueprint'/Game/KatanaProject/UI/UI_StartMenu.UI_StartMenu_C'"));
	if (StartHUDClass.Succeeded())
		m_StartHUDClass = StartHUDClass.Class;
}

void AKatanaStartGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AKatanaStartGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	if (IsValid(m_StartHUDClass))
	{
		m_StartHUD = Cast<UStartMenuHUD>(CreateWidget(GetWorld(), m_StartHUDClass));
	
		// 여러개의 위젯이 존재할 경우 ZOrder를 이용하여 나중에 그려질 위젯을 정해줄 수 있다.
		if (m_StartHUD)
			m_StartHUD->AddToViewport();
	}
	//
	// APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	//
	// FInputModeGameAndUI FMode;
	//
	// Controller->SetInputMode(FMode);
	// Controller->bShowMouseCursor = true;	// 마우스 커서 출력 -> SelectPlayerController의 생성자에서도 호출 가능
}
