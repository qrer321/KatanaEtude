// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInfo.h"

DEFINE_LOG_CATEGORY(KatnaProject)

void PrintViewport(float _fTime, const FColor& _Color, const FString& _Text)
{
	GEngine->AddOnScreenDebugMessage(-1, _fTime, _Color, _Text);
}
