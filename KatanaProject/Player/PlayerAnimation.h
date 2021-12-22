// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimation.generated.h"

UENUM(Blueprintable)
enum class EPlayerAnimType : uint8
{
	Ground,
	Jump,
	Fall,
};

UENUM(Blueprintable)
enum class EPlayerStateType : uint8
{
	Idle,
	Defence
};

UENUM(Blueprintable)
enum class EMovementType : uint8
{
	Walking,
	Running,
	//Sprinting,
	//Crawling
};

UENUM(Blueprintable)
enum class EWeaponType : uint8
{
	None,
	Katana,
	Bow,
};

UCLASS()
class KATANAPROJECT_API UPlayerAnimation : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayerAnimation();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_SpeedFwd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_SpeedSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool m_IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EPlayerAnimType m_AnimType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EPlayerStateType m_StateType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EDefenceDirType m_DefenceDirType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EMovementType m_MovementType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EWeaponType m_WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_FallRecoveryAdditive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_ForwardAxisValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_SideAxisValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_BlendValue;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//bool m_Hit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool m_IsDeflect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool m_CanOtherAction;

	UPROPERTY()
	UAnimSequenceBase* m_RunStartAnim;
	UPROPERTY()
	UAnimSequenceBase* m_RunStopAnim;
	UPROPERTY()
	UAnimSequenceBase* m_WalkStartAnim;
	UPROPERTY()
	UAnimSequenceBase* m_WalkStopAnim;

	
	// Foot IK
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_Displacement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_IKInterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRotator m_RFootRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRotator m_LFootRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_RIK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_LIK;


public:
	void ChangeAnimType(EPlayerAnimType Type) { m_AnimType = Type; }
	void ChangeStateType(EPlayerStateType Type) { m_StateType = Type; }
	void ChangeMovementType(EMovementType Type) { m_MovementType = Type; }
	void ChangeWeaponType(EWeaponType Type) { m_WeaponType = Type; }
	void ChangeDefenceDirType(EDefenceDirType Type) { m_DefenceDirType = Type; }

	EPlayerAnimType GetAnimType() const { return m_AnimType; }
	EPlayerStateType GetStateType() const { return m_StateType; }
	EMovementType GetMovementType() const { return m_MovementType; }
	EWeaponType GetWeaponType() const { return m_WeaponType; }
	EDefenceDirType GetDefenceDirType() const { return m_DefenceDirType; }

	UFUNCTION(BlueprintCallable)
	bool IsEnemyLocked() const;

	void SetFallRecoveryAdditive(float Additive) { m_FallRecoveryAdditive = Additive; }

	//void SetPlayerHit(bool Hit) { m_Hit = Hit; }

	void SetDeflect(bool Deflect) { m_IsDeflect = Deflect; }
	bool IsDeflect() const { return m_IsDeflect; }

	void SetOtherAction(bool CanAction) { m_CanOtherAction = CanAction; }
	bool CanBeOtherAction();
	bool IsOtherAction() const { return m_CanOtherAction; }

	// Foot IK
	void FootPlacement(float DeltaSecond);
	void Dis(FName SocketName, bool& BlockingHit, float& Distance) const;
	void FootLineTrace(FName SocketName, bool& BlockingHit, float& Distance, FVector& Normal) const;

public:
	void StartAndStopAnim();

	UFUNCTION(BlueprintCallable)
	float GetForwardAxisValue();

	UFUNCTION(BlueprintCallable)
	float GetSideAxisValue();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION()
	void AnimNotify_JumpStart();

	UFUNCTION()
	void AnimNotify_JumpEnd();

	UFUNCTION()
	void AnimNotify_LandEnd();

	UFUNCTION()
	void AnimNotify_WeaponEquip();

	UFUNCTION()
	void AnimNotify_WeaponUnEquip();

	UFUNCTION()
	void AnimNotify_CheckNextCombo() const;

	UFUNCTION()
	void AnimNotify_ComboCheck() const;

	UFUNCTION()
	void AnimNotify_OnCollStart_Attack();

	UFUNCTION()
	void AnimNotify_OnCollEnd_Attack();

	UFUNCTION()
	void AnimNotify_IsMove();
	
	UFUNCTION()
	void AnimNotify_HitEnd();

	UFUNCTION()
	void AnimNotify_ChangeType();

	UFUNCTION()
	void AnimNotify_Dodge_Start();

	UFUNCTION()
	void AnimNotify_CanBeOtherAction();

	UFUNCTION()
	void AnimNotify_Dodge_End();

	UFUNCTION()
	void AnimNotify_DropletParticle();

	UFUNCTION()
	void AnimNotify_Dismember();
};