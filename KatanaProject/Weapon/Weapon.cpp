// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "../Player/PrototypeCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//m_Body = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	//SetRootComponent(m_Body);

	//m_Body->SetCollisionProfileName(TEXT("NoCollision"));
	m_Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	m_IsOutput = true;
}

void AWeapon::SetWeaponOwner(ACharacter* WeaponOwner)
{
	m_WeaponOwner = WeaponOwner;
}

template <typename T>
T* AWeapon::GetWeaponOwner() const
{
	return Cast<T>(m_WeaponOwner);
}

void AWeapon::SetMesh(const FString& Path)
{
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *Path);
	m_Mesh->SetStaticMesh(Mesh);
}

void AWeapon::SetWeaponOutput(bool IsOutput)
{
	//m_IsOutput = IsOutput;

	if (IsOutput)
		SetActorHiddenInGame(false);
	else
		SetActorHiddenInGame(true);
}

void AWeapon::SetWeaponSimulate() const
{
	GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	
	GetMesh()->SetCollisionProfileName(TEXT("Dismembered"));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

