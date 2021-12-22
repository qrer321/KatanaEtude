// Fill out your copyright notice in the Description page of Project Settings.


#include "DeflectCameraShake.h"

UDeflectCameraShake::UDeflectCameraShake()
{
	OscillationDuration = 0.2f;

	OscillationBlendInTime = 0.4f;
	OscillationBlendOutTime = 0.4f;

	LocOscillation.Y.Amplitude = 10.f;
	LocOscillation.Y.Frequency = 100.f;
	LocOscillation.Y.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;

	LocOscillation.Z.Amplitude = 10.f;
	LocOscillation.Z.Frequency = 100.f;
	LocOscillation.Z.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;

	bSingleInstance = true;
}
