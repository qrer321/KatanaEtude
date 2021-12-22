// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../GameInfo.h"
#include "../Effect/DropletEffect.h"
#include "GameFramework/Character.h"
#include "PrototypeCharacter.generated.h"

UCLASS(config=MainAsset)
class APrototypeCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	FPlayerInfo m_PlayerInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UPlayerAnimation* m_AnimInstance;

	FVector m_DesiredDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* m_FallRecoveryMontage;

	int32 m_EquipMontageIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> m_WeaponEquipRootArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> m_WeaponUnEquipRootArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> m_WeaponEquipInPlaceArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> m_WeaponUnEquipInPlaceArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> m_DeflectLArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> m_DeflectRArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> m_ChangeDefenceArray;	// Idle to Defence / Defence to Idle

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> m_HitArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> m_AvoidArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> m_ExecutionArray;

	TArray<FComboAttack> m_ComboAttackArray;
	int32 m_ComboIndex;
	int32 m_AttackIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AEnemyCharacter* m_EnemyActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool m_IsEnemyLocked;

	FTimerHandle m_EnemyLockTimer;

	FTimerHandle m_DeflectTimer;
	FTimerHandle m_StillDeflectTimer;
	float m_DeflectTime;

	bool m_Dodge;


	// Perilous 빌보드 매테리얼 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMaterialBillboardComponent* m_PerilousBillBoard;

	int32 m_PerilousCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADropletEffect> m_DropletClass;

	// 체간 시스템
	bool m_SPRecover;
	float m_SPRecoverTime;	
	FTimerHandle m_SPRecoverTimer;

	int32 m_ExecutionIndex;

public:
	const FVector& GetDesiredVelocity();
	
	bool IsEnemyLocked() const { return m_IsEnemyLocked; }

	const FPlayerInfo& GetPlayerInfo() const { return m_PlayerInfo; }

	class AEnemyCharacter* GetEnemyActorSet() const { return m_EnemyActor; }
	void SetEnemyActor(class AEnemyCharacter* Enemy);
	void ResetEnemyActor(); 

	void SetDodge(bool Dodge) { m_Dodge = Dodge; }
	bool IsDodge() const { return m_Dodge; }

	void IsDeflect();
	void StillDeflect();

	void SetPerilousVisible(bool Visible) const;
	void SetPerilousCount(int32 Count) { m_PerilousCount = Count; }
	int32 GetPerilousCount() const { return m_PerilousCount; }

	TSubclassOf<ADropletEffect> GetDropletClass() const { return m_DropletClass; }

	void ActionDismemberToVictim();
	
public:
	APrototypeCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void ToggleRunKey();
	virtual void CameraLockKey();
	void WeaponEquipKey();
	void AttackKey();
	void AttackValue(float Value);
	
	virtual void DeflectOnKey();
	virtual void DeflectOffKey();
	void DeflectKey(float Value) {}

	void DodgeKey();

	void ReturnMainMenu();

public:
	virtual void Attack() {}
	virtual void DoAttackCombo() {}
	virtual void Deflect() {}
	virtual void Avoid() {}
	virtual void Slide() {}

	virtual void BeginColl() {}
	virtual void EndColl() {}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
