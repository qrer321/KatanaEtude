// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Weapon.h"
#include "Katana.generated.h"

UCLASS()
class KATANAPROJECT_API AKatana : public AWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKatana();

protected:
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
