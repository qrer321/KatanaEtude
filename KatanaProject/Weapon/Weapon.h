// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS(BlueprintType)
class KATANAPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ACharacter* m_WeaponOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool m_IsOutput;

public:
	void SetWeaponOwner(ACharacter* WeaponOwner);

	template <typename T>
	T* GetWeaponOwner() const;

	void SetMesh(const FString& Path);
	UStaticMeshComponent* GetMesh() const { return m_Mesh; }

	UFUNCTION()
	void SetWeaponOutput(bool IsOutput); // true : Rendering, false : NoRendering

	void SetWeaponSimulate() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
