// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMenuHUD.h"

void UStartMenuHUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	m_StartButton = Cast<UButton>(GetWidgetFromName(TEXT("StartButton")));
	m_PracButton = Cast<UButton>(GetWidgetFromName(TEXT("PracticeButton")));
	m_QuitButton = Cast<UButton>(GetWidgetFromName(TEXT("QuitButton")));

	m_StartButton->OnClicked.AddDynamic(this, &UStartMenuHUD::StartButtonClick);
	m_PracButton->OnClicked.AddDynamic(this, &UStartMenuHUD::PracButtonClick);
	m_QuitButton->OnClicked.AddDynamic(this, &UStartMenuHUD::QuitButtonClick);
}

void UStartMenuHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UStartMenuHUD::StartButtonClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Sci-Fi_Level"));
}

void UStartMenuHUD::PracButtonClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("tpexmaplemap"));
}

void UStartMenuHUD::QuitButtonClick()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}
