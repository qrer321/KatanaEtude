// Fill out your copyright notice in the Description page of Project Settings.


#include "DropletEffect.h"

// Sets default values
ADropletEffect::ADropletEffect()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Default Offset Setting
	m_DecalFadeTimerOffset = 10.f;

	m_ParticleSystemNames = EParticleSystemName::Aortic_Spurt;
	m_DecalTimeout = 120.f;
	m_ParticleAmountCoefficient = 1.f;
	m_GravityCoefficient = 1.f;
	m_ParticleLifetimeCoefficient = 1.f;
	m_EmitterDurationCoefficient = 1.f;
	m_DecalScaleCoefficient = 1.f;
	m_DropletScaleCoefficient = 1.f;
	m_DropletTailScaleCoefficient = 1.f;
	m_SpriteScaleCoefficient = 1.f;
	m_VelocityDirectionalCoefficient = 1.f;
	m_VelocityStrengthCoefficient = 1.f;
	m_BoxLocationScaleCoefficient = 1.f;
	m_CollisionTimerOffset = 0.1f;
	m_CollisionBoundsCoefficient = 0.025f;
	m_ActorTimeout = 0.f;

	m_SKOwner = nullptr;
	m_SMOwner = nullptr;
	m_SKActive = false;
	m_SMActive = false;

	m_OffsetLocation = 0.f;
	m_OffsetRotation = FRotator::ZeroRotator;
}

// Called when the game starts or when spawned
void ADropletEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADropletEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_SKActive && m_SKOwner)
	{
		const FVector SocketLocation = m_SKOwner->GetSocketLocation(m_SocketName);
		const FRotator SocketRotation = m_SKOwner->GetSocketRotation(m_SocketName);
		
		SetActorLocation(SocketLocation);
		SetActorRotation(SocketRotation);
	}

	if (m_SMActive && m_SMOwner)
	{
		// 필요에 따라서 Offset 위치 설정
		const FVector CompLocation = m_SMOwner->GetComponentLocation();
		const FRotator CompRotation = m_SMOwner->GetComponentRotation();

		SetActorLocation(CompLocation);
		SetActorRotation(CompRotation);
	}
}

