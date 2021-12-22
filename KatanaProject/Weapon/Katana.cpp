// Fill out your copyright notice in the Description page of Project Settings.


#include "Katana.h"
#include "../Player/PrototypeCharacter.h"
#include "../Enemy/EnemyCharacter.h"
#include "../Effect/AttackCameraShake.h"

AKatana::AKatana()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> KatanaMesh(TEXT("StaticMesh'/Game/GhostSamurai_Bundle/GhostSamurai/Weapon/Mesh/Katana/SM_Katana01.SM_Katana01'"));
	if (KatanaMesh.Succeeded())
		m_Mesh->SetStaticMesh(KatanaMesh.Object);

	m_Mesh->SetGenerateOverlapEvents(true);
	m_Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void AKatana::BeginPlay()
{
	Super::BeginPlay();

	m_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKatana::NotifyActorBeginOverlap(AActor* OtherActor)
{	
	Super::NotifyActorBeginOverlap(OtherActor);
	//
	// APrototypeCharacter* PlayerCharacter = Cast<APrototypeCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	// if (!PlayerCharacter)
	// 	return;
	//
	// const FPlayerInfo& PlayerInfo = PlayerCharacter->GetPlayerInfo();

	int AttackDamage = 0;
	const FDamageEvent DmgEvent;

	// 적과 플레이어 둘 다 오버랩함수를 사용하기 때문에 캐스트를 통해 알맞은 값을 넘겨주자
	APrototypeCharacter* PlayerCharacter = Cast<APrototypeCharacter>(m_WeaponOwner);
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(m_WeaponOwner);
	if (PlayerCharacter)
	{
		PrintViewport(2.f, FColor::Red, TEXT("PlayerAttack"));
		
		AttackDamage = PlayerCharacter->GetPlayerInfo().Attack;
		
		AEnemyCharacter* TargetCharacter = Cast<AEnemyCharacter>(OtherActor);
		if (TargetCharacter)
		{
			TargetCharacter->TakeDamage(AttackDamage, DmgEvent, PlayerCharacter->GetController(), PlayerCharacter);

			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(UAttackCameraShake::StaticClass());
		}
	}
	
	if (EnemyCharacter)
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		AttackDamage = EnemyCharacter->GetEnemyInfo().Attack;
	
		APrototypeCharacter* TargetCharacter = Cast<APrototypeCharacter>(OtherActor);
		if (TargetCharacter && !TargetCharacter->IsDodge())
		{
			PrintViewport(2.f, FColor::Red, TEXT("EnemyAttack"));
			TargetCharacter->TakeDamage(AttackDamage, DmgEvent, EnemyCharacter->GetController(), EnemyCharacter);
	
			//GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(UAttackCameraShake::StaticClass());
		}
	}
}

void AKatana::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	//PrintViewport(1.f, FColor::Red, TEXT("End Overlap"));
}


void AKatana::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
