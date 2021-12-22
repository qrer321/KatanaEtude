// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "DropletEffect.generated.h"

UENUM(BlueprintType)
enum class EParticleSystemName : uint8
{
	Aortic_Spurt,
	Blood_Pool,
	Punch,
	Puncture,
	Entrance_Wound_Pistol,
	Entrance_Wound_Rifle,
	Entrance_Wound_Shotgun,
	Exit_Wound_Pistol,
	Exit_Wound_Rifle,
	Exit_Wound_Shotgun,
	Loop_Bleeding,
	Loop_Excessive_Dismembered_Spurt,
	Loop_Mild_Dismembered_Spurt,
	Timed_Bleeding,
	Timed_Excessive_Dismembered_Spurt,
	Timed_Mild_Dismembered_Spurt
};

UCLASS()
class KATANAPROJECT_API ADropletEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropletEffect();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer Setting", meta = (AllowPrivateAccess = "true"))
	float m_DecalFadeTimerOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Setting", meta = (AllowPrivateAccess = "true"))
	EParticleSystemName m_ParticleSystemNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Setting", meta = (AllowPrivateAccess = "true"))
	float m_DecalTimeout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Setting", meta = (AllowPrivateAccess = "true"))
	float m_ParticleAmountCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Setting", meta = (AllowPrivateAccess = "true"))
	float m_GravityCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Setting", meta = (AllowPrivateAccess = "true"))
	float m_ParticleLifetimeCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Setting", meta = (AllowPrivateAccess = "true"))
	float m_EmitterDurationCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Setting", meta = (AllowPrivateAccess = "true"))
	float m_DecalScaleCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Setting", meta = (AllowPrivateAccess = "true"))
	float m_DropletScaleCoefficient;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Setting", meta = (AllowPrivateAccess = "true"))
	float m_DropletTailScaleCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Setting", meta = (AllowPrivateAccess = "true"))
	float m_SpriteScaleCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Setting", meta = (AllowPrivateAccess = "true"))
	float m_VelocityDirectionalCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Setting", meta = (AllowPrivateAccess = "true"))
	float m_VelocityStrengthCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Setting", meta = (AllowPrivateAccess = "true"))
	float m_BoxLocationScaleCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Setting", meta = (AllowPrivateAccess = "true"))
	float m_CollisionTimerOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Setting", meta = (AllowPrivateAccess = "true"))
	float m_CollisionBoundsCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle Setting", meta = (AllowPrivateAccess = "true"))
	float m_ActorTimeout;

protected:
	UPROPERTY()
	USkeletalMeshComponent* m_SKOwner;
	UPROPERTY()
	UStaticMeshComponent* m_SMOwner;

	FName m_SocketName;
	
	bool m_SKActive;
	bool m_SMActive;

	float m_OffsetLocation;
	FRotator m_OffsetRotation;

public:
	void SetDropletName(EParticleSystemName Name) { m_ParticleSystemNames = Name; }
	void SetEmitterDuration(float Time) { m_EmitterDurationCoefficient = Time; }
	void SetDecalTimeout(float Time) { m_DecalTimeout = Time; }

public:
	void SetSKOwner(USkeletalMeshComponent* SkeletalMesh, FName SocketName) { m_SKOwner = SkeletalMesh; m_SKActive = true; m_SocketName = SocketName; }
	void SetSMOwner(UStaticMeshComponent* StaticMesh) { m_SMOwner = StaticMesh; m_SMActive = true; }
	void SetOffsetLocation(float Offset) { m_OffsetLocation = Offset; }
	void SetOffsetRotation(FRotator Offset) { m_OffsetRotation = Offset; }
	
protected:
	// Called when the game starts or when spawned

	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
