// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Camera/CameraShake.h"
#include "DeflectCameraShake.generated.h"

UCLASS()
class KATANAPROJECT_API UDeflectCameraShake : public UMatineeCameraShake
{
	GENERATED_BODY()

public:
	UDeflectCameraShake();
};
