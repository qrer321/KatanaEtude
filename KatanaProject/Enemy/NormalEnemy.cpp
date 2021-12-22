// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalEnemy.h"
#include "EnemyAIController.h"

ANormalEnemy::ANormalEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	m_EnemyInfoName = TEXT("NormalEnemy");
	
	AIControllerClass = AEnemyAIController::StaticClass();
}

void ANormalEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ANormalEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
