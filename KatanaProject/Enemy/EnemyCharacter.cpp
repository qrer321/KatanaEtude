// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "EnemyAnimInstance.h"
#include "EnemyAIController.h"
#include "../KatanaProjectGameInstance.h"
#include "../KatanaProjectGameMode.h"
#include "../Player/PrototypeCharacter.h"
#include "../Weapon/Katana.h"
#include "../Weapon/Weapon.h"
#include "../Effect/NormalEffect.h"
#include "../UI/HPBar.h"
#include "../UI/EnemyHUD.h"
#include "../UI/EnemyStaminaHUD.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 충돌 Profile을 Enemy로 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/GhostSamurai_Bundle/GhostSamurai/Character/Mesh/SK_GhostSamurai_katana.SK_GhostSamurai_katana'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	m_HeadOnly = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_HeadOnly"));
	m_HeadOnly->SetupAttachment(GetMesh());
	m_HeadOnly->SetVisibility(false);
	m_HeadOnly->SetCollisionProfileName(TEXT("Dismembered"));
	m_HeadOnly->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HeadOnlyAsset(TEXT("SkeletalMesh'/Game/KatanaProject/Dismemberment/head_only.head_only'"));
	if (HeadOnlyAsset.Succeeded())
		m_HeadOnly->SetSkeletalMesh(HeadOnlyAsset.Object);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyOnlyAsset(TEXT("SkeletalMesh'/Game/KatanaProject/Dismemberment/body_only.body_only'"));
	if (BodyOnlyAsset.Succeeded())
		m_BodyOnly = BodyOnlyAsset.Object;

	m_ExecutionBillBoard = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("Billboard"));
	m_ExecutionBillBoard->SetupAttachment(GetCapsuleComponent());
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>	MtrlAsset(TEXT("Material'/Game/KatanaProject/Material/MT_ExecutionAlert.MT_ExecutionAlert'"));
	if (MtrlAsset.Succeeded())
		m_ExecutionBillBoard->AddElement(MtrlAsset.Object, nullptr, false, 32.f, 32.f, nullptr);

	m_ExecutionBillBoard->SetVisibility(false);

	
	static ConstructorHelpers::FClassFinder<ADropletEffect> DropletClass(TEXT("Blueprint'/Game/KatanaProject/Effect/BP_Droplets.BP_Droplets_C'"));
	if (DropletClass.Succeeded())
		m_DropletClass = DropletClass.Class;


	{
		static ConstructorHelpers::FObjectFinder<UAnimMontage> HitAdditiveAsset(
			TEXT("AnimMontage'/Game/KatanaProject/Enemy/Animation/AMEnemyHitAdditive.AMEnemyHitAdditive'"));
		if (HitAdditiveAsset.Succeeded())
		{
			m_HitAddtiveMontage = HitAdditiveAsset.Object;

			m_HitAddtiveMontage->BlendIn.SetBlendOption(EAlphaBlendOption::Cubic);
			m_HitAddtiveMontage->BlendIn.SetBlendTime(0.1f);

			m_HitAddtiveMontage->BlendOut.SetBlendOption(EAlphaBlendOption::Cubic);
			m_HitAddtiveMontage->BlendOut.SetBlendTime(0.3f);
		}

		static ConstructorHelpers::FObjectFinder<UAnimMontage> GroggyAsset(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Hit/GhostSamurai_APose_Large_Hit_2_Root_Montage.GhostSamurai_APose_Large_Hit_2_Root_Montage'"));
		if (GroggyAsset.Succeeded())
			m_GroggyMontage = GroggyAsset.Object;

		const FComboAttack Combo1;
		m_ComboAttackArray.Add(Combo1);

		// AttackCombo 애니메이션
		static ConstructorHelpers::FObjectFinder<UAnimMontage> GroundAttack01_1(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/AMGroundAttack01_1.AMGroundAttack01_1'"));
		if (GroundAttack01_1.Succeeded())
			m_ComboAttackArray[0].AttackArray.Add(GroundAttack01_1.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> GroundAttack01_2(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/AMGroundAttack01_2.AMGroundAttack01_2'"));
		if (GroundAttack01_2.Succeeded())
			m_ComboAttackArray[0].AttackArray.Add(GroundAttack01_2.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> GroundAttack01_3(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/AMGroundAttack01_3.AMGroundAttack01_3'"));
		if (GroundAttack01_3.Succeeded())
			m_ComboAttackArray[0].AttackArray.Add(GroundAttack01_3.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> GroundAttack01_4(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/AMGroundAttack01_4.AMGroundAttack01_4'"));
		if (GroundAttack01_4.Succeeded())
			m_ComboAttackArray[0].AttackArray.Add(GroundAttack01_4.Object);


		const FComboAttack Combo2;
		m_ComboAttackArray.Add(Combo2);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> GroundAttack02_1(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/AMGroundAttack02_1.AMGroundAttack02_1'"));
		if (GroundAttack02_1.Succeeded())
			m_ComboAttackArray[1].AttackArray.Add(GroundAttack02_1.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> GroundAttack02_2(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/AMGroundAttack02_2.AMGroundAttack02_2'"));
		if (GroundAttack02_2.Succeeded())
			m_ComboAttackArray[1].AttackArray.Add(GroundAttack02_2.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> GroundAttack02_3(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/AMGroundAttack02_3.AMGroundAttack02_3'"));
		if (GroundAttack02_3.Succeeded())
			m_ComboAttackArray[1].AttackArray.Add(GroundAttack02_3.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> GroundAttack02_4(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/AMGroundAttack02_4.AMGroundAttack02_4'"));
		if (GroundAttack02_4.Succeeded())
			m_ComboAttackArray[1].AttackArray.Add(GroundAttack02_4.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> GroundAttack02_5(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/AMGroundAttack02_5.AMGroundAttack02_5'"));
		if (GroundAttack02_5.Succeeded())
			m_ComboAttackArray[1].AttackArray.Add(GroundAttack02_5.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> GroundAttack02_6(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/AMGroundAttack02_6.AMGroundAttack02_6'"));
		if (GroundAttack02_6.Succeeded())
			m_ComboAttackArray[1].AttackArray.Add(GroundAttack02_6.Object);


		const FComboAttack Combo3;
		m_ComboAttackArray.Add(Combo3);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> GroundAttack03_1(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/AMGroundAttack03_1.AMGroundAttack03_1'"));
		if (GroundAttack03_1.Succeeded())
			m_ComboAttackArray[2].AttackArray.Add(GroundAttack03_1.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> GroundAttack03_2(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/AMGroundAttack03_2.AMGroundAttack03_2'"));
		if (GroundAttack03_2.Succeeded())
			m_ComboAttackArray[2].AttackArray.Add(GroundAttack03_2.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> GroundAttack03_3(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/AMGroundAttack03_3.AMGroundAttack03_3'"));
		if (GroundAttack03_3.Succeeded())
			m_ComboAttackArray[2].AttackArray.Add(GroundAttack03_3.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> GroundAttack03_4(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/AMGroundAttack03_4.AMGroundAttack03_4'"));
		if (GroundAttack03_4.Succeeded())
			m_ComboAttackArray[2].AttackArray.Add(GroundAttack03_4.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> GroundAttack03_5(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/AMGroundAttack03_5.AMGroundAttack03_5'"));
		if (GroundAttack03_5.Succeeded())
			m_ComboAttackArray[2].AttackArray.Add(GroundAttack03_5.Object);


		// Heavy Attack
		static ConstructorHelpers::FObjectFinder<UAnimMontage> HeavyAttack_1(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/GhostSamurai_APose_SPAttack01_Root_Montage.GhostSamurai_APose_SPAttack01_Root_Montage'"));
		if (HeavyAttack_1.Succeeded())
			m_HeavyAttackArray.Add(HeavyAttack_1.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> HeavyAttack_2(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/GhostSamurai_APose_SPAttack02_Root_Montage.GhostSamurai_APose_SPAttack02_Root_Montage'"));
		if (HeavyAttack_2.Succeeded())
			m_HeavyAttackArray.Add(HeavyAttack_2.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> HeavyAttack_3(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/GhostSamurai_APose_SPAttack03_Root_Montage.GhostSamurai_APose_SPAttack03_Root_Montage'"));
		if (HeavyAttack_3.Succeeded())
			m_HeavyAttackArray.Add(HeavyAttack_3.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> HeavyAttack_4(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/GhostSamurai_APose_SPAttack04_Root_Montage.GhostSamurai_APose_SPAttack04_Root_Montage'"));
		if (HeavyAttack_4.Succeeded())
			m_HeavyAttackArray.Add(HeavyAttack_4.Object);

		// static ConstructorHelpers::FObjectFinder<UAnimMontage> HeavyAttack_5(TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/GhostSamurai_APose_SPAttack05_Root_Montage.GhostSamurai_APose_SPAttack05_Root_Montage'"));
		// if (HeavyAttack_5.Succeeded())
		// 	m_HeavyAttackArray.Add(HeavyAttack_5.Object);
		//
		// static ConstructorHelpers::FObjectFinder<UAnimMontage> HeavyAttack_6(TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Attack/Root/GhostSamurai_APose_SPAttack06_Root_Montage.GhostSamurai_APose_SPAttack06_Root_Montage'"));
		// if (HeavyAttack_6.Succeeded())
		// 	m_HeavyAttackArray.Add(HeavyAttack_6.Object);


		// Defence Anim Montage
		static ConstructorHelpers::FObjectFinder<UAnimMontage> IdleToDefenceAsset_L(
			TEXT(
				"AnimMontage'/Game/KatanaProject/Player/animation/Deflect/GhostSamurai_APose2DefenseL_Root_Montage_RTI.GhostSamurai_APose2DefenseL_Root_Montage_RTI'"));
		if (IdleToDefenceAsset_L.Succeeded())
			m_ChangeDefenceArray.Add(IdleToDefenceAsset_L.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> IdleToDefenceAsset_R(
			TEXT(
				"AnimMontage'/Game/KatanaProject/Player/animation/Deflect/GhostSamurai_APose2DefenseR_Root_Montage_RTI.GhostSamurai_APose2DefenseR_Root_Montage_RTI'"));
		if (IdleToDefenceAsset_R.Succeeded())
			m_ChangeDefenceArray.Add(IdleToDefenceAsset_R.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> DefenceToIdleAsset_L(
			TEXT(
				"AnimMontage'/Game/KatanaProject/Player/animation/Deflect/GhostSamurai_DefenseL2APose_Root_Montage_RTI.GhostSamurai_DefenseL2APose_Root_Montage_RTI'"));
		if (DefenceToIdleAsset_L.Succeeded())
			m_ChangeDefenceArray.Add(DefenceToIdleAsset_L.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> DefenceToIdleAsset_R(
			TEXT(
				"AnimMontage'/Game/KatanaProject/Player/animation/Deflect/GhostSamurai_DefenseR2APose_Root_Montage_RTI.GhostSamurai_DefenseR2APose_Root_Montage_RTI'"));
		if (DefenceToIdleAsset_R.Succeeded())
			m_ChangeDefenceArray.Add(DefenceToIdleAsset_R.Object);


		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeflectLAsset_1(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Deflect/GhostSamurai_DefenseL_Hit01_Root_Montage.GhostSamurai_DefenseL_Hit01_Root_Montage'"));
		if (DeflectLAsset_1.Succeeded())
			m_DeflectLArray.Add(DeflectLAsset_1.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeflectLAsset_2(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Deflect/GhostSamurai_DefenseL_Hit02_Root_Montage.GhostSamurai_DefenseL_Hit02_Root_Montage'"));
		if (DeflectLAsset_2.Succeeded())
			m_DeflectLArray.Add(DeflectLAsset_2.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeflectLAsset_3(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Deflect/GhostSamurai_DefenseL_Parry01_Root_Montage.GhostSamurai_DefenseL_Parry01_Root_Montage'"));
		if (DeflectLAsset_3.Succeeded())
			m_DeflectLArray.Add(DeflectLAsset_3.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeflectLAsset_4(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Deflect/GhostSamurai_DefenseL_Parry02_Root_Montage.GhostSamurai_DefenseL_Parry02_Root_Montage'"));
		if (DeflectLAsset_4.Succeeded())
			m_DeflectLArray.Add(DeflectLAsset_4.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeflectLAsset_5(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Deflect/GhostSamurai_DefenseL_Parry03_Root_Montage.GhostSamurai_DefenseL_Parry03_Root_Montage'"));
		if (DeflectLAsset_5.Succeeded())
			m_DeflectLArray.Add(DeflectLAsset_5.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeflectLAsset_6(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Deflect/GhostSamurai_DefenseL_Parry_L2R_Down_Root_Montage.GhostSamurai_DefenseL_Parry_L2R_Down_Root_Montage'"));
		if (DeflectLAsset_6.Succeeded())
			m_DeflectLArray.Add(DeflectLAsset_6.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeflectLAsset_7(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Deflect/GhostSamurai_DefenseL_Parry_L2R_Up_Root_Montage.GhostSamurai_DefenseL_Parry_L2R_Up_Root_Montage'"));
		if (DeflectLAsset_7.Succeeded())
			m_DeflectLArray.Add(DeflectLAsset_7.Object);


		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeflectRAsset_1(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Deflect/GhostSamurai_DefenseR_Hit01_Root_Montage.GhostSamurai_DefenseR_Hit01_Root_Montage'"));
		if (DeflectRAsset_1.Succeeded())
			m_DeflectRArray.Add(DeflectRAsset_1.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeflectRAsset_2(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Deflect/GhostSamurai_DefenseR_Hit02_Root_Montage.GhostSamurai_DefenseR_Hit02_Root_Montage'"));
		if (DeflectRAsset_2.Succeeded())
			m_DeflectRArray.Add(DeflectRAsset_2.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeflectRAsset_3(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Deflect/GhostSamurai_DefenseR_Parry01_Root_Montage.GhostSamurai_DefenseR_Parry01_Root_Montage'"));
		if (DeflectRAsset_3.Succeeded())
			m_DeflectRArray.Add(DeflectRAsset_3.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeflectRAsset_4(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Deflect/GhostSamurai_DefenseR_Parry02_Root_Montage.GhostSamurai_DefenseR_Parry02_Root_Montage'"));
		if (DeflectRAsset_4.Succeeded())
			m_DeflectRArray.Add(DeflectRAsset_4.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeflectRAsset_5(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Deflect/GhostSamurai_DefenseR_Parry03_Root_Montage.GhostSamurai_DefenseR_Parry03_Root_Montage'"));
		if (DeflectRAsset_5.Succeeded())
			m_DeflectRArray.Add(DeflectRAsset_5.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeflectRAsset_6(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Deflect/GhostSamurai_DefenseR_Parry_R2L_Down_Root_Montage.GhostSamurai_DefenseR_Parry_R2L_Down_Root_Montage'"));
		if (DeflectRAsset_6.Succeeded())
			m_DeflectRArray.Add(DeflectRAsset_6.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeflectRAsset_7(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Deflect/GhostSamurai_DefenseR_Parry_R2L_Up_Root_Montage.GhostSamurai_DefenseR_Parry_R2L_Up_Root_Montage'"));
		if (DeflectRAsset_7.Succeeded())
			m_DeflectRArray.Add(DeflectRAsset_7.Object);


		// Executed Anim Montage
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ExecutedAsset_1(TEXT("AnimMontage'/Game/KatanaProject/Enemy/Animation/Executed/GhostSamurai_Executed01_Montage.GhostSamurai_Executed01_Montage'"));
		if (ExecutedAsset_1.Succeeded())
			m_ExecutedArray.Add(ExecutedAsset_1.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ExecutedAsset_2(TEXT("AnimMontage'/Game/KatanaProject/Enemy/Animation/Executed/GhostSamurai_Executed02_Montage.GhostSamurai_Executed02_Montage'"));
		if (ExecutedAsset_2.Succeeded())
			m_ExecutedArray.Add(ExecutedAsset_2.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ExecutedAsset_3(TEXT("AnimMontage'/Game/KatanaProject/Enemy/Animation/Executed/GhostSamurai_Executed03_Montage.GhostSamurai_Executed03_Montage'"));
		if (ExecutedAsset_3.Succeeded())
			m_ExecutedArray.Add(ExecutedAsset_3.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ExecutedAsset_4(TEXT("AnimMontage'/Game/KatanaProject/Enemy/Animation/Executed/GhostSamurai_Executed04_Montage.GhostSamurai_Executed04_Montage'"));
		if (ExecutedAsset_4.Succeeded())
			m_ExecutedArray.Add(ExecutedAsset_4.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ExecutedAsset_5(TEXT("AnimMontage'/Game/KatanaProject/Enemy/Animation/Executed/GhostSamurai_Executed05_Montage.GhostSamurai_Executed05_Montage'"));
		if (ExecutedAsset_5.Succeeded())
			m_ExecutedArray.Add(ExecutedAsset_5.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ExecutedAsset_6(TEXT("AnimMontage'/Game/KatanaProject/Enemy/Animation/Executed/GhostSamurai_Executed06_Montage.GhostSamurai_Executed06_Montage'"));
		if (ExecutedAsset_6.Succeeded())
			m_ExecutedArray.Add(ExecutedAsset_6.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ExecutedAsset_7(TEXT("AnimMontage'/Game/KatanaProject/Enemy/Animation/Executed/GhostSamurai_Executed07_Montage.GhostSamurai_Executed07_Montage'"));
		if (ExecutedAsset_7.Succeeded())
			m_ExecutedArray.Add(ExecutedAsset_7.Object);


		// Death Anim Montage
		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathAsset_1(TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Die/GhostSamurai_APose_Die01_Root_Montage.GhostSamurai_APose_Die01_Root_Montage'"));
		if (DeathAsset_1.Succeeded())
			m_DeathArray.Add(DeathAsset_1.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathAsset_2(TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Die/GhostSamurai_APose_Die02_Root_Montage.GhostSamurai_APose_Die02_Root_Montage'"));
		if (DeathAsset_2.Succeeded())
			m_DeathArray.Add(DeathAsset_2.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathAsset_3(TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Die/GhostSamurai_APose_Die03_Root_Montage.GhostSamurai_APose_Die03_Root_Montage'"));
		if (DeathAsset_3.Succeeded())
			m_DeathArray.Add(DeathAsset_3.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathAsset_4(TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Die/GhostSamurai_APose_Die04_Root_Montage.GhostSamurai_APose_Die04_Root_Montage'"));
		if (DeathAsset_4.Succeeded())
			m_DeathArray.Add(DeathAsset_4.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathAsset_5(TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Die/GhostSamurai_APose_Die05_Root_Montage.GhostSamurai_APose_Die05_Root_Montage'"));
		if (DeathAsset_5.Succeeded())
			m_DeathArray.Add(DeathAsset_5.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathAsset_6(TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Die/GhostSamurai_APose_Die06_Root_Montage.GhostSamurai_APose_Die06_Root_Montage'"));
		if (DeathAsset_6.Succeeded())
			m_DeathArray.Add(DeathAsset_6.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathAsset_7(TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Die/GhostSamurai_APose_Die07_Root_Montage.GhostSamurai_APose_Die07_Root_Montage'"));
		if (DeathAsset_7.Succeeded())
			m_DeathArray.Add(DeathAsset_7.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathAsset_8(TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Die/GhostSamurai_APose_Die08_Root_Montage.GhostSamurai_APose_Die08_Root_Montage'"));
		if (DeathAsset_8.Succeeded())
			m_DeathArray.Add(DeathAsset_8.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathAsset_9(TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Die/GhostSamurai_APose_Die09_Root_Montage.GhostSamurai_APose_Die09_Root_Montage'"));
		if (DeathAsset_9.Succeeded())
			m_DeathArray.Add(DeathAsset_9.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathAsset_10(TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Die/GhostSamurai_APose_Die10_Root_Montage.GhostSamurai_APose_Die10_Root_Montage'"));
		if (DeathAsset_10.Succeeded())
			m_DeathArray.Add(DeathAsset_10.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathAsset_11(TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Die/GhostSamurai_APose_Die11_Root_Montage.GhostSamurai_APose_Die11_Root_Montage'"));
		if (DeathAsset_11.Succeeded())
			m_DeathArray.Add(DeathAsset_11.Object);
		
	}
	

	AIControllerClass = AEnemyAIController::StaticClass();
	
	m_IsCoolTime = false;
	m_AttackCoolTime = 0.f;

	m_SPRecover = true;
	m_SPRecoverTime = 0.f;
	//m_SPRecoverMaxTime = 3.f;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	m_AnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	// 칼, 칼집 액터 생성
	m_Katana = GetWorld()->SpawnActor<AKatana>(AKatana::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, param);
	m_Sheath = GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, param);
	
	m_Katana->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Katana"));
	m_Sheath->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Sheath"));
	
	m_Sheath->SetMesh(TEXT("StaticMesh'/Game/GhostSamurai_Bundle/GhostSamurai/Weapon/Mesh/Katana/SM_Scabbard01.SM_Scabbard01'"));

	// 이후 세이브 로드 기능이 생겼을 때는
	// 저장될 당시 캐릭터 상태를 확인하여 처리할 필요가 있다
	m_Katana->SetWeaponOutput(true);
	m_Sheath->SetWeaponOutput(true);

	m_Katana->SetWeaponOwner(this);
	m_Sheath->SetWeaponOwner(this);

	m_Katana->GetMesh()->SetCollisionProfileName(TEXT("EnemyAttack"));

	UKatanaProjectGameInstance* GameInstance = Cast<UKatanaProjectGameInstance>(GetWorld()->GetGameInstance());	
	if (GameInstance)
	{
		const FEnemyTableInfo* EnemyInfo = GameInstance->FindEnemyInfo(m_EnemyInfoName);
	
		if (EnemyInfo)
		{
			m_EnemyInfo.Name = EnemyInfo->Name;
			m_EnemyInfo.Attack = EnemyInfo->Attack;
			m_EnemyInfo.Armor = EnemyInfo->Armor;
			m_EnemyInfo.HP = EnemyInfo->HP;
			m_EnemyInfo.HPMax = EnemyInfo->HPMax;
			m_EnemyInfo.SP = EnemyInfo->SP;
			m_EnemyInfo.SPMax = EnemyInfo->SPMax;
			m_EnemyInfo.SPRecoverMaxTime = EnemyInfo->SPRecoverMaxTime;
			m_EnemyInfo.Level = EnemyInfo->Level;
			m_EnemyInfo.Exp = EnemyInfo->Exp;
			m_EnemyInfo.Gold = EnemyInfo->Gold;
			m_EnemyInfo.AttackDistance = EnemyInfo->AttackDistance;
			m_EnemyInfo.AttackAngle = EnemyInfo->AttackAngle;
			m_EnemyInfo.AttackSpeed = EnemyInfo->AttackSpeed;
			m_EnemyInfo.MoveSpeed = EnemyInfo->MoveSpeed;
			m_EnemyInfo.TraceDistance = EnemyInfo->TraceDistance;
			m_EnemyInfo.WatchDistance = EnemyInfo->WatchDistance;
			m_EnemyInfo.AttackCoolTime = EnemyInfo->AttackCoolTime;
		}
	}

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	if (Movement->bOrientRotationToMovement)
		Movement->bOrientRotationToMovement = false;
	
	// GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DissolveEnable"), -1.f);
	//
	// m_fDissolveRange = m_fDissolveMax - m_fDissolveMin;
	// m_fDissolve = m_fDissolveMax;
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!m_AnimInstance)
		return;

	// 처형될 경우 Physics 시뮬레이션이 동작하기 때문에
	// 캐릭터 메시는 랙돌이 되어 가만히 있지만
	// 캡슐 컴포넌트는 아직 살아있기 때문에 위치가 계속 변화한다
	// 해당 문제점을 해결하기 위함
	if (m_AnimInstance->IsExecuted())
	{
		FVector PelvisLocation = GetMesh()->GetSocketLocation(TEXT("Pelvis"));
		PelvisLocation += FVector(0.f, 0.f, 97.f);
		GetCapsuleComponent()->SetWorldLocation(PelvisLocation);
	}
	
	if (m_EnemyInfo.HP <= 0)
	{
		if (GetAnimType() != EEnemyAnimType::Death)
			ChangeAnimType(EEnemyAnimType::Death);
		
		return;
	}

	if (m_IsCoolTime)
	{
		//m_AnimInstance->ChangeAnimType(EEnemyAnimType::Idle);
		
		m_AttackCoolTime += DeltaTime;
		
		if (m_AttackCoolTime >= m_EnemyInfo.AttackCoolTime)
		{
			m_IsCoolTime = false;
			m_AttackCoolTime = 0.f;
		}
	}
		

	if (EEnemyAnimType::Attack == m_AnimInstance->GetAnimType())
	{
		// 공격 몽타주 실행하기
		if (m_AttackEnd && !m_IsCoolTime)
		{
			m_AttackEnd = false;

			if (m_AttackIndex >= m_ComboAttackArray[m_ComboIndex].AttackArray.Num())
				m_AttackIndex = 0;
			
			m_AnimInstance->Montage_Play(m_ComboAttackArray[m_ComboIndex].AttackArray[m_AttackIndex]);

			m_AttackIndex = (m_AttackIndex + 1) % m_ComboAttackArray[m_ComboIndex].AttackArray.Num();
		}
	}

	if (EEnemyAnimType::HeavyAttack == m_AnimInstance->GetAnimType())
	{
		if (m_AttackEnd && !m_IsCoolTime)
		{
			//SetAttackCoolTime();
			m_AttackEnd = false;

			// 강공격 실행
			const int32 HeavyAttack = FMath::RandRange(0, m_HeavyAttackArray.Num() - 1);
			m_AnimInstance->Montage_Play(m_HeavyAttackArray[HeavyAttack]);
		}
	}

	// 체간 회복
	EnemyStaminaRecoverFunc(DeltaTime);
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float Damage =  Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (0.f == Damage || m_EnemyInfo.HP == 0)
		return 0.f;

	Damage = Damage - m_EnemyInfo.Armor;
	Damage = Damage < 1.f ? 1.f : Damage;	// 아머가 아무리 높아도 최소 1은 들어가게끔 설정

	AKatanaProjectGameMode* GameMode = Cast<AKatanaProjectGameMode>(GetWorld()->GetAuthGameMode());
	const UMainHUD* MainHUD = nullptr;
	const UEnemyHUD* EnemyHUD = nullptr;
	const UEnemyStaminaHUD* EnemyStaminaHUD = nullptr;
	if (IsValid(GameMode))
	{
		MainHUD = GameMode->GetMainHUD();
		if (IsValid(MainHUD))
		{
			EnemyHUD = GameMode->GetMainHUD()->GetEnemyHUD();
			EnemyStaminaHUD = GameMode->GetMainHUD()->GetEnemyStaminaHUD();	
		}
	}

	// 그로기 상태일 경우 체력만 깎는다
	// 가능하면 애디티브 애니메이션 추가가 필요해보임
	if (m_AnimInstance->IsGroggy())
	{
		m_EnemyInfo.HP -= Damage;
		if (IsValid(EnemyHUD))
			EnemyHUD->SetHPPercent(m_EnemyInfo.HP / (float)m_EnemyInfo.HPMax);

		if (IsValid(m_DropletClass))
		{
			const FRotator PlayerRotation = DamageCauser->GetActorRotation();
			const FRotator PlayerYawRotation = FRotator(0.f, PlayerRotation.Yaw, 0.f);
			
			FTransform EffectTrans;
			EffectTrans.SetLocation(GetActorLocation());
			EffectTrans.SetRotation(PlayerYawRotation.Quaternion());
			EffectTrans.SetScale3D(FVector(1.f, 1.f, 1.f));
			
			ADropletEffect* DropletEffect = GetWorld()->SpawnActorDeferred<ADropletEffect>(m_DropletClass, EffectTrans);
			DropletEffect->SetDropletName(EParticleSystemName::Exit_Wound_Shotgun);
			DropletEffect->SetDecalTimeout(20.f);
			DropletEffect->FinishSpawning(EffectTrans);
		}
	}
	else
	{
		// Hit 당할 경우 몽타주 stop
		m_AnimInstance->Montage_Stop(0.4f);
		m_Katana->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 플레이어를 공격한 Enemy를 바라보게 만들고
		const FVector& CauserLocation = DamageCauser->GetActorLocation();
		const FRotator ToCauserRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CauserLocation);
		const FRotator YawRotation = FRotator(0, ToCauserRotation.Yaw, 0);
		SetActorRotation(YawRotation);

		if (m_AnimInstance->GetAnimType() == EEnemyAnimType::Defence)
		{
			// Defence 했다고 AI에게 알림
			AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
			if (AIController)
				AIController->GetBlackboardComponent()->SetValueAsBool("IsDefence", true);
			
			// Hit 애니메이션과 Defence 애니메이션을 블렌드하여 재생
			const int DeflectIndex = FMath::FRandRange(0, 6);
			m_AnimInstance->Montage_Play(m_HitAddtiveMontage);

			if (m_AnimInstance->GetDefenceDirType() == EDefenceDirType::DefenceL)
				m_AnimInstance->Montage_Play(m_DeflectLArray[DeflectIndex], 1.f, EMontagePlayReturnType::MontageLength,
				                             0, false);
			else if (m_AnimInstance->GetDefenceDirType() == EDefenceDirType::DefenceR)
				m_AnimInstance->Montage_Play(m_DeflectRArray[DeflectIndex], 1.f, EMontagePlayReturnType::MontageLength,
				                             0, false);

			//GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(UDeflectCameraShake::StaticClass());

			const FVector ParticleLocation = GetMesh()->GetSocketLocation(TEXT("DeflectParticle"));
			const FRotator ParticleRotation = GetMesh()->GetSocketRotation(TEXT("DeflectParticle"));

			FActorSpawnParameters param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ANormalEffect* DeflectParticle = GetWorld()->SpawnActor<ANormalEffect>(
				ANormalEffect::StaticClass(), ParticleLocation, ParticleRotation, param);
			//DeflectParticle->LoadParticle(TEXT("ParticleSystem'/Game/VFXSparkPack/Particles/VFX_ElectricBreak_mine.VFX_ElectricBreak_mine'"));
			DeflectParticle->LoadParticleAsync(TEXT("DeflectParticle"));

			m_SPRecover = false;
			m_SPRecoverTime = 0.f;

			m_EnemyInfo.SP -= Damage * 0.5f;
			if (IsValid(MainHUD) && IsValid(EnemyStaminaHUD))
			{
				APrototypeCharacter* PlayerCharacter = Cast<APrototypeCharacter>(DamageCauser);
				if (PlayerCharacter)
				{
					if (this == PlayerCharacter->GetEnemyActorSet())
						MainHUD->SetEnemyStaminaVisibility(true);
				}

				EnemyStaminaHUD->SetStaminaPercent(m_EnemyInfo.SP / (float)m_EnemyInfo.SPMax);
			}
		}
		else
		{
			m_AnimInstance->SetHit();
			HitAdditiveMontage();

			//const int32 ToDefensePer = FMath::RandRange(0, 100);
			//if (ToDefensePer > 30)
			//{
			// 60의 값보다 클 경우 Defence로 애니메이션 변경
			// ...이것도 결국 8way라서 블렌드스페이스를 만들어서 AI를 어떻게 처리해야할까
			// 랜덤한 포지션으로 이동하게 해야하나? 아니면 Side Value만을 가지고 움직이게 해야하나?
			m_AnimInstance->ChangeAnimType(EEnemyAnimType::Defence);
			//}

			if (IsValid(m_DropletClass))
			{
				const FRotator PlayerRotation = DamageCauser->GetActorRotation();
				const FRotator PlayerYawRotation = FRotator(0.f, PlayerRotation.Yaw, 0.f);

				FTransform EffectTrans;
				EffectTrans.SetLocation(GetActorLocation()); // Impact Point로 생성해야 한다
				EffectTrans.SetRotation(PlayerYawRotation.Quaternion());
				EffectTrans.SetScale3D(FVector(1.f, 1.f, 1.f));

				ADropletEffect* DropletEffect = GetWorld()->SpawnActorDeferred<ADropletEffect>(
					m_DropletClass, EffectTrans);
				DropletEffect->SetDropletName(EParticleSystemName::Exit_Wound_Shotgun);
				DropletEffect->SetDecalTimeout(20.f);
				DropletEffect->FinishSpawning(EffectTrans);
			}

			m_EnemyInfo.HP -= Damage;
			if (IsValid(EnemyHUD))
				EnemyHUD->SetHPPercent(m_EnemyInfo.HP / (float)m_EnemyInfo.HPMax);
		}
	}
	
	if (m_EnemyInfo.HP <= 0)
	{
		m_EnemyInfo.HP = 0;
		
		// Enemy 사망
		const int32 DeathIndex = FMath::RandRange(0, m_DeathArray.Num() - 1);
		m_AnimInstance->Montage_Play(m_DeathArray[DeathIndex]);
		m_AnimInstance->ChangeAnimType(EEnemyAnimType::Death);	// -> Death 몽타주 돌면서 EnemyExecuted 함수가 호출되게 해두었다
		
		m_ExecutionBillBoard->SetVisibility(false);
	}

	if (m_EnemyInfo.SP < 0)
	{
		m_EnemyInfo.SP = 0;

		// Large Hit 애니메이션 재생
		m_AnimInstance->Montage_Play(m_GroggyMontage);

		// Enemy는 그로기 상태에 들어간다
		// 그로기 상태에서는 5초? 정도 SP 회복이 불가하지만 더 이상 SP가 깎이지 않는다
		// -> Large Hit 애니메이션 여러번 재생되는 것을 막기 위함
		// 5초 이후에는 빠른 속도로 SP를 회복한다
		m_AnimInstance->SetGroggy(true);
		
		// 그로기 상태에서는 플레이어에게 인살이 가능한 상태라고 빌보드 UI로 알려주어야 한다
		m_ExecutionBillBoard->SetVisibility(true);

		// 해당 상태에서 플레이어는 인살이 가능한 가장 가까운 Enemy를 찾아
		// 애님 시퀀스를 강제 재생한다
		// 당하는 Enemy 또한 동일한 이름의 인살 시퀀스를 재생한다
	}

	return Damage;
}

void AEnemyCharacter::BeginColl()
{
	m_Katana->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AEnemyCharacter::EndColl()
{
	m_Katana->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyCharacter::HitAdditiveMontage()
{
	if (!m_HitAddtiveMontage)
		return;

	// 공격을 받을때마다 HitAdditiveMontage를 실행할 수 있도록 만든다
	m_AnimInstance->Montage_SetPosition(m_HitAddtiveMontage, 0.f);
	m_AnimInstance->Montage_Play(m_HitAddtiveMontage);
}

void AEnemyCharacter::SetSPToMax()
{
	m_EnemyInfo.SP = m_EnemyInfo.SPMax;

	AKatanaProjectGameMode* GameMode = Cast<AKatanaProjectGameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(GameMode))
	{
		const UMainHUD* MainHUD = GameMode->GetMainHUD();
		if (IsValid(MainHUD))
		{
			const UEnemyStaminaHUD* StaminaHUD = MainHUD->GetEnemyStaminaHUD();
			if (IsValid(StaminaHUD))
			{		
				StaminaHUD->SetStaminaPercent(m_EnemyInfo.SP / (float)m_EnemyInfo.SPMax);
			}
		}
	}
}

void AEnemyCharacter::Dismember()
{
	// 신체결손 기능
	// Collision 및 Visibility 활성화
	m_HeadOnly->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	m_HeadOnly->SetVisibility(true);

	// 굴러다니게 하기 위한 물리 시뮬레이션
	//m_HeadOnly->SetSimulatePhysics(true);
	m_HeadOnly->SetAllBodiesBelowSimulatePhysics(TEXT("pelvis"), true);
	m_HeadOnly->SetAllBodiesBelowPhysicsBlendWeight(TEXT("pelvis"), 1.f);

	// 회전값 추가
	FVector AngularVelocity = m_HeadOnly->GetPhysicsAngularVelocityInDegrees();
	FVector LinearVelocity = m_HeadOnly->GetPhysicsLinearVelocity();
	
	AngularVelocity.Y += 1000.f;
	LinearVelocity.Z += 10.f;
	
	m_HeadOnly->SetPhysicsAngularVelocityInDegrees(AngularVelocity, true);
	m_HeadOnly->SetPhysicsLinearVelocity(LinearVelocity, true);
	

	// 기존 SK 바디를 몸통만 있는 SK로 변경
	GetMesh()->SetSkeletalMesh(m_BodyOnly, false);

	// Head와 Neck에 DropletEffect 추가
	const FVector HeadLocation = m_HeadOnly->GetSocketLocation(TEXT("NeckSocket"));
	const FVector NeckLocation = GetMesh()->GetSocketLocation(TEXT("neck_01"));
	
	FTransform EffectTrans;
	
	EffectTrans.SetLocation(HeadLocation);
	ADropletEffect* HeadDropletEffect = GetWorld()->SpawnActorDeferred<ADropletEffect>(m_DropletClass, EffectTrans);
	HeadDropletEffect->SetDropletName(EParticleSystemName::Timed_Mild_Dismembered_Spurt);
	HeadDropletEffect->SetSKOwner(m_HeadOnly, TEXT("NeckSocket"));
	HeadDropletEffect->SetEmitterDuration(6.f);
	HeadDropletEffect->SetDecalTimeout(20.f);
	HeadDropletEffect->FinishSpawning(EffectTrans);

	EffectTrans.SetLocation(NeckLocation);
	ADropletEffect* NeckDropletEffect = GetWorld()->SpawnActorDeferred<ADropletEffect>(m_DropletClass, EffectTrans);
	NeckDropletEffect->SetDropletName(EParticleSystemName::Timed_Excessive_Dismembered_Spurt);
	NeckDropletEffect->SetSKOwner(GetMesh(), TEXT("neck_01"));
	NeckDropletEffect->SetEmitterDuration(6.f);
	NeckDropletEffect->SetDecalTimeout(20.f);
	NeckDropletEffect->FinishSpawning(EffectTrans);
}

void AEnemyCharacter::StopAIController(const FString& StopReason) const
{
	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject("Target", nullptr);
		AIController->BrainComponent->StopLogic(TEXT("Dead"));
	}
}

void AEnemyCharacter::EnemyExecuted()
{
	// Enemy 사망
	m_EnemyInfo.HP = 0;
	StopAIController(TEXT("Dead"));
	
	APrototypeCharacter* PlayerCharacter = Cast<APrototypeCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (PlayerCharacter)
		PlayerCharacter->ResetEnemyActor();
			
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// 죽을 경우 Katana Mesh 물리 시뮬 돌리기
	m_Katana->SetWeaponSimulate();
	m_Sheath->SetWeaponSimulate();

	//GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("pelvis"), true);
	GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(TEXT("pelvis"), 1.f);
	m_AnimInstance->SetExecuted(true);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Ragdoll"));
	
	GetWorld()->GetTimerManager().SetTimer(m_BloodpoolTimer, this, &AEnemyCharacter::CreateBloodPool, 1.f, false, 3.f);
}

void AEnemyCharacter::CreateBloodPool()
{
	if (IsValid(m_DropletClass))
	{
		FCollisionQueryParams param(NAME_None, false, this);
		FHitResult result;
		bool Hit = GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(), GetActorLocation() + FVector(0.f, 0.f, -300.f), ECC_GameTraceChannel5, param);
		if (Hit)
		{
			FTransform EffectTrans;
			EffectTrans.SetLocation(result.ImpactPoint);

			ADropletEffect* DropletEffect = GetWorld()->SpawnActorDeferred<ADropletEffect>(m_DropletClass, EffectTrans);
			DropletEffect->SetDropletName(EParticleSystemName::Blood_Pool);
			DropletEffect->FinishSpawning(EffectTrans);
		}
	}
}

void AEnemyCharacter::EnemyStaminaRecoverFunc(float DeltaTime)
{
	if (m_SPRecover)
	{
		if (!GetWorldTimerManager().TimerExists(m_SPRecoverTimer))
			GetWorldTimerManager().SetTimer(m_SPRecoverTimer, this, &AEnemyCharacter::SPRecover, 0.2f, true);
		
		if (m_EnemyInfo.SP >= m_EnemyInfo.SPMax)
		{
			m_EnemyInfo.SP = m_EnemyInfo.SPMax;
			m_SPRecover = false;

			GetWorldTimerManager().ClearTimer(m_SPRecoverTimer);
			return;
		}

		AKatanaProjectGameMode* GameMode = Cast<AKatanaProjectGameMode>(GetWorld()->GetAuthGameMode());
		if (IsValid(GameMode))
		{
			const UMainHUD* MainHUD = GameMode->GetMainHUD();
			if (IsValid(MainHUD))
			{
				const UEnemyStaminaHUD* StaminaHUD = MainHUD->GetEnemyStaminaHUD();
				if (IsValid(StaminaHUD))
				{
					APrototypeCharacter* PlayerCharacter = Cast<APrototypeCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
					if (PlayerCharacter)
					{
						if (this == PlayerCharacter->GetEnemyActorSet())
							MainHUD->SetEnemyStaminaVisibility(true);
					}
					
					StaminaHUD->SetStaminaPercent(m_EnemyInfo.SP / (float)m_EnemyInfo.SPMax);
				}
			}
		}
	}
	else
	{
		m_SPRecoverTime += DeltaTime;
		if (m_SPRecoverTime >= m_EnemyInfo.SPRecoverMaxTime)
		{
			m_SPRecoverTime = 0.f;
			m_SPRecover = true;

			if (m_EnemyInfo.SP >= m_EnemyInfo.SPMax)
			{
				AKatanaProjectGameMode* GameMode = Cast<AKatanaProjectGameMode>(GetWorld()->GetAuthGameMode());
				if (IsValid(GameMode))
				{
					const UMainHUD* MainHUD = GameMode->GetMainHUD();
					if (IsValid(MainHUD))
					{
						APrototypeCharacter* PlayerCharacter = Cast<APrototypeCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
						if (PlayerCharacter)
						{
							if (this == PlayerCharacter->GetEnemyActorSet())
								MainHUD->SetEnemyStaminaVisibility(false);
						}
					}
				
					GetWorldTimerManager().ClearTimer(m_SPRecoverTimer);
				}
			}
		}
	}
}
