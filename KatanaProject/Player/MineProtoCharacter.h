// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "PrototypeCharacter.h"
#include "MineProtoCharacter.generated.h"

UCLASS()
class KATANAPROJECT_API AMineProtoCharacter : public APrototypeCharacter
{
	GENERATED_BODY()

public:
	AMineProtoCharacter();

protected:
	UPROPERTY()
	class AKatana* m_Katana;

	UPROPERTY()
	class AWeapon* m_Sheath;
	
	UPROPERTY()
	class AWeapon* m_AllKatana;

	//class AWeapon* m_Bow;


	bool m_Attack;
	bool m_ContinueAttack;

protected:
	virtual void Attack() override;

public:
	virtual void DoAttackCombo() override;

	void ComboCheck() { m_ContinueAttack = false; }
	void AttackReset();

	bool IsAnyExecutedActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//class AWeapon* GetKatana() const { return m_Katana.GetDefaultObject(); }
	class AKatana* GetKatana() const {return m_Katana; }
	class AWeapon* GetSheath() const { return m_Sheath; }
	class AWeapon* GetAllKatana() const { return m_AllKatana; }

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	void JumpKey();
	void FallRecovery();
	virtual void CameraLockKey() override;
	virtual void Avoid() override;
	virtual void Slide() override;

	virtual void BeginColl() override;
	virtual void EndColl() override;

	virtual void DeflectOnKey() override;
	virtual void DeflectOffKey() override;

protected:
	void LockTimer();
	void StaminaRecoverFunc(float DeltaTime);
	void SPRecover() { m_PlayerInfo.SP += 1; }

	class AEnemyCharacter* DetectClosestEnemyFunc(const float DetectLength, const float DetectAngle, bool DrawDebug) const;
};
