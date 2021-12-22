// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnemyCharacter.h"
#include "NormalEnemy.generated.h"

UCLASS()
class KATANAPROJECT_API ANormalEnemy : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	ANormalEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
