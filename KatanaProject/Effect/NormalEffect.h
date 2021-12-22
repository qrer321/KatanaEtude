// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "NormalEffect.generated.h"

UCLASS()
class KATANAPROJECT_API ANormalEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANormalEffect();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* m_pParticleSystem;

	FSoftObjectPath m_AsyncParticlePath;
	FSoftObjectPath m_AsyncSoundPath;

	// 언리얼에서 제공하는 SharedPtr(스마트포인터)
	TSharedPtr<FStreamableHandle>	m_AsyncParticleLoadHandle;	// 비동기로 로딩을 할 때
	TSharedPtr<FStreamableHandle>	m_AsyncSoundLoadHandle;

	bool m_bAssetLoop;

public:
	void AssetLoop() { m_bAssetLoop = true; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void LoadParticle(const FString& _Path);
	void LoadParticleAsync(const FString& _Name);	// 비동기 에셋 로딩
	void LoadParticleAsyncComplete();				// 비동기 로딩이 완료되고 호출 될 함수

	void LoadSound(const FString& _Path);
	void LoadSoundAsync(const FString& _Name);
	void LoadSoundAsyncComplete();				// 비동기 로딩이 완료되고 호출 될 함수

	UFUNCTION()
	void ParticleFinish(UParticleSystemComponent* _pParticle);
};
