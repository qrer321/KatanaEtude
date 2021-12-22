// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EEnemyAnimType : uint8
{
	Idle,
	Walk,
	Run,
	SideL,
	SideR,
	Attack,
	HeavyAttack,
	Defence,
	Death,
};

UCLASS()
class KATANAPROJECT_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEnemyAnimInstance();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool m_OnGround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EEnemyAnimType m_AnimType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EDefenceDirType m_DefenceDirType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_Hit;

	UPROPERTY()
	bool m_PerilousVisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool m_Groggy;
	float m_GroggyElapsedTime;
	float m_GroggyMaxTime;
	FTimerHandle m_GroggyTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool m_Executed;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float _fDeltaSeconds) override;

public:
	void ChangeAnimType(EEnemyAnimType _eType) { m_AnimType = _eType; }
	void ChangeDefenceDirType(EDefenceDirType Type) { m_DefenceDirType = Type; }
	
	EEnemyAnimType GetAnimType() const { return m_AnimType; }
	EDefenceDirType GetDefenceDirType() const { return m_DefenceDirType; }

	void SetHit();
	void SetPerilousVisible(bool Visible) { m_PerilousVisible = Visible; }
	void SetGroggy(bool Groggy);
	void SetExecuted(bool Executed) { m_Executed = Executed; }
	
	float GetHit() const { return m_Hit; }
	bool IsPerilousVisible() const { return m_PerilousVisible; }
	bool IsGroggy() const { return m_Groggy; }
	bool IsExecuted() const { return m_Executed; }

	void GroggyTimer();
	
public:
	UFUNCTION()
	void AnimNotify_HitEnd();

	UFUNCTION()
	void AnimNotify_CheckNextCombo();

	UFUNCTION()
	void AnimNotify_ComboCheck();

	UFUNCTION()
	void AnimNotify_OnCollStart_Attack();

	UFUNCTION()
	void AnimNotify_OnCollEnd_Attack();

	UFUNCTION()
	void AnimNotify_Perilous_Start();

	UFUNCTION()
	void AnimNotify_Perilous_End();

	UFUNCTION()
	void AnimNotify_IsExecuted();
};
