// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "EnemyAnimInstance.h"
#include "../Effect/DropletEffect.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class KATANAPROJECT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* m_HeadOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	USkeletalMesh* m_BodyOnly;
	
	UPROPERTY()
	class AKatana* m_Katana;

	UPROPERTY()
	class AWeapon* m_Sheath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	FEnemyInfo m_EnemyInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	FString m_EnemyInfoName;

	UPROPERTY()
	class UEnemyAnimInstance* m_AnimInstance;

	TArray<FComboAttack> m_ComboAttackArray;
	int32 m_ComboIndex;
	int32 m_AttackIndex;

	UPROPERTY()
	TArray<UAnimMontage*> m_HeavyAttackArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	bool m_AttackEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	bool m_IsCoolTime;
	float m_AttackCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* m_HitAddtiveMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* m_GroggyMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> m_DeflectLArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> m_DeflectRArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> m_ChangeDefenceArray; // Idle to Defence / Defence to Idle

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> m_ExecutedArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> m_DeathArray;

	// 체간 시스템
	bool m_SPRecover;
	float m_SPRecoverTime;
	FTimerHandle m_SPRecoverTimer;

	// 인살 빌보드 매테리얼 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMaterialBillboardComponent* m_ExecutionBillBoard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADropletEffect> m_DropletClass;
	FTimerHandle m_BloodpoolTimer;

	//class AMonsterSpawnPoint* m_pSpawnPoint;

	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	// TArray<FVector> m_PatrolArray;
	// int32 m_iPatrolIndex;

public:
	UFUNCTION(BlueprintCallable)
	void ChangeAnimType(EEnemyAnimType Type) const { m_AnimInstance->ChangeAnimType(Type); }

	void SPRecover() { m_EnemyInfo.SP += 1; }
	void SetSPToMax();
	void SetSPRecover(bool Recover) { m_SPRecover = Recover; }

	void SetExecutionVisibility(bool Visible) const { m_ExecutionBillBoard->SetVisibility(Visible); }
	void PlayExecutedAnimMontage(const int32& MontageIndex) { m_AnimInstance->Montage_Play(m_ExecutedArray[MontageIndex]); }

	void EnemyExecuted();
	void Dismember();
	void StopAIController(const FString& StopReason) const;
	
	void CreateBloodPool();
	
public:
	class UEnemyAnimInstance* GetAnimInstance() const { return m_AnimInstance; }
	const FEnemyInfo& GetEnemyInfo() const { return m_EnemyInfo; }
	EEnemyAnimType GetAnimType() const { return m_AnimInstance->GetAnimType(); }
	TArray<FComboAttack> GetComboAttackArray() const { return m_ComboAttackArray; }
	
	float GetHit() const { return m_AnimInstance->GetHit(); }
	bool IsCoolTime() const { return m_IsCoolTime; }
	bool IsGroggy() const { return m_AnimInstance->IsGroggy(); }

protected:
	void HitAdditiveMontage();
	void EnemyStaminaRecoverFunc(float DeltaTime);

public:
	void ResetAttackIndex() { m_AttackIndex = 0; }
	void SetAttackEnd() { m_AttackEnd = true; }
	void SetAttackCoolTime() { m_IsCoolTime = true; }

	virtual void BeginColl();
	virtual void EndColl();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         class AController* EventInstigator, AActor* DamageCauser) override;
};
