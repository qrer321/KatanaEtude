// Fill out your copyright notice in the Description page of Project Settings.


#include "MineProtoCharacter.h"
#include "PlayerAnimation.h"
#include "../KatanaProjectGameInstance.h"
#include "../KatanaProjectGameMode.h"
#include "../Enemy/EnemyCharacter.h"
#include "../Weapon/Weapon.h"
#include "../Weapon/Katana.h"
#include "../Effect/DeflectCameraShake.h"
#include "../Effect/NormalEffect.h"
#include "../UI/MainHUD.h"
#include "../UI/CharacterHUD.h"
#include "../UI/StaminaHUD.h"

AMineProtoCharacter::AMineProtoCharacter()
{	
	{
		static ConstructorHelpers::FObjectFinder<UAnimMontage> FallRecoveryAsset(
			TEXT("AnimMontage'/Game/KatanaProject/Player/animation/AMJumpEndAdditive.AMJumpEndAdditive'"));
		if (FallRecoveryAsset.Succeeded())
		{
			m_FallRecoveryMontage = FallRecoveryAsset.Object;
			m_FallRecoveryMontage->BlendIn.SetBlendOption(EAlphaBlendOption::Cubic);
			m_FallRecoveryMontage->BlendIn.SetBlendTime(0.01f);

			m_FallRecoveryMontage->BlendOut.SetBlendOption(EAlphaBlendOption::Cubic);
			m_FallRecoveryMontage->BlendOut.SetBlendTime(0.01f);
		}

		// Equip, UnEquip Root 애니메이션
		static ConstructorHelpers::FObjectFinder<UAnimMontage> WeaponEquipAssetRoot1(
			TEXT(
				"AnimMontage'/Game/KatanaProject/Player/animation/WeaponEquip/Root/AMWeaponEquipRoot_1.AMWeaponEquipRoot_1'"));
		if (WeaponEquipAssetRoot1.Succeeded())
			m_WeaponEquipRootArray.Add(WeaponEquipAssetRoot1.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> WeaponEquipAssetRoot2(
			TEXT(
				"AnimMontage'/Game/KatanaProject/Player/animation/WeaponEquip/Root/AMWeaponEquipRoot_2.AMWeaponEquipRoot_2'"));
		if (WeaponEquipAssetRoot2.Succeeded())
			m_WeaponEquipRootArray.Add(WeaponEquipAssetRoot2.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> WeaponUnEquipAssetRoot1(
			TEXT(
				"AnimMontage'/Game/KatanaProject/Player/animation/WeaponEquip/Root/AMWeaponUnEquipRoot_1.AMWeaponUnEquipRoot_1'"));
		if (WeaponUnEquipAssetRoot1.Succeeded())
			m_WeaponUnEquipRootArray.Add(WeaponUnEquipAssetRoot1.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> WeaponUnEquipAssetRoot2(
			TEXT(
				"AnimMontage'/Game/KatanaProject/Player/animation/WeaponEquip/Root/AMWeaponUnEquipRoot_2.AMWeaponUnEquipRoot_2'"));
		if (WeaponUnEquipAssetRoot2.Succeeded())
			m_WeaponUnEquipRootArray.Add(WeaponUnEquipAssetRoot2.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> WeaponUnEquipAssetRoot3(
			TEXT(
				"AnimMontage'/Game/KatanaProject/Player/animation/WeaponEquip/Root/AMWeaponUnEquipRoot_3.AMWeaponUnEquipRoot_3'"));
		if (WeaponUnEquipAssetRoot3.Succeeded())
			m_WeaponUnEquipRootArray.Add(WeaponUnEquipAssetRoot3.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> WeaponUnEquipAssetRoot4(
			TEXT(
				"AnimMontage'/Game/KatanaProject/Player/animation/WeaponEquip/Root/AMWeaponUnEquipRoot_4.AMWeaponUnEquipRoot_4'"));
		if (WeaponUnEquipAssetRoot4.Succeeded())
			m_WeaponUnEquipRootArray.Add(WeaponUnEquipAssetRoot4.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> WeaponUnEquipAssetRoot5(
			TEXT(
				"AnimMontage'/Game/KatanaProject/Player/animation/WeaponEquip/Root/AMWeaponUnEquipRoot_5.AMWeaponUnEquipRoot_5'"));
		if (WeaponUnEquipAssetRoot5.Succeeded())
			m_WeaponUnEquipRootArray.Add(WeaponUnEquipAssetRoot5.Object);


		// Equip, UnEquip InPlace 애니메이션
		static ConstructorHelpers::FObjectFinder<UAnimMontage> WeaponEquipAssetInPlace1(
			TEXT(
				"AnimMontage'/Game/KatanaProject/Player/animation/WeaponEquip/Inpalce/AMWeaponEquipInPlace_1.AMWeaponEquipInPlace_1'"));
		if (WeaponEquipAssetInPlace1.Succeeded())
			m_WeaponEquipInPlaceArray.Add(WeaponEquipAssetInPlace1.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> WeaponEquipAssetInPlace2(
			TEXT(
				"AnimMontage'/Game/KatanaProject/Player/animation/WeaponEquip/Inpalce/AMWeaponEquipInPlace_2.AMWeaponEquipInPlace_2'"));
		if (WeaponEquipAssetInPlace2.Succeeded())
			m_WeaponEquipInPlaceArray.Add(WeaponEquipAssetInPlace2.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> WeaponUnEquipAssetInPlace1(
			TEXT(
				"AnimMontage'/Game/KatanaProject/Player/animation/WeaponEquip/Inpalce/AMWeaponUnEquipInPlace_1.AMWeaponUnEquipInPlace_1'"));
		if (WeaponUnEquipAssetInPlace1.Succeeded())
			m_WeaponUnEquipInPlaceArray.Add(WeaponUnEquipAssetInPlace1.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> WeaponUnEquipAssetInPlace2(
			TEXT(
				"AnimMontage'/Game/KatanaProject/Player/animation/WeaponEquip/Inpalce/AMWeaponUnEquipInPlace_2.AMWeaponUnEquipInPlace_2'"));
		if (WeaponUnEquipAssetInPlace2.Succeeded())
			m_WeaponUnEquipInPlaceArray.Add(WeaponUnEquipAssetInPlace2.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> WeaponUnEquipAssetInPlace3(
			TEXT(
				"AnimMontage'/Game/KatanaProject/Player/animation/WeaponEquip/Inpalce/AMWeaponUnEquipInPlace_3.AMWeaponUnEquipInPlace_3'"));
		if (WeaponUnEquipAssetInPlace3.Succeeded())
			m_WeaponUnEquipInPlaceArray.Add(WeaponUnEquipAssetInPlace3.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> WeaponUnEquipAssetInPlace4(
			TEXT(
				"AnimMontage'/Game/KatanaProject/Player/animation/WeaponEquip/Inpalce/AMWeaponUnEquipInPlace_4.AMWeaponUnEquipInPlace_4'"));
		if (WeaponUnEquipAssetInPlace4.Succeeded())
			m_WeaponUnEquipInPlaceArray.Add(WeaponUnEquipAssetInPlace4.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> WeaponUnEquipAssetInPlace5(
			TEXT(
				"AnimMontage'/Game/KatanaProject/Player/animation/WeaponEquip/Inpalce/AMWeaponUnEquipInPlace_5.AMWeaponUnEquipInPlace_5'"));
		if (WeaponUnEquipAssetInPlace5.Succeeded())
			m_WeaponUnEquipInPlaceArray.Add(WeaponUnEquipAssetInPlace5.Object);


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


		// Hit Anim Montage
		static ConstructorHelpers::FObjectFinder<UAnimMontage> HitAsset_1(
			TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Hit/AMHitF.AMHitF'"));
		if (HitAsset_1.Succeeded())
			m_HitArray.Add(HitAsset_1.Object);


		// Avoid Anim Montage
		static ConstructorHelpers::FObjectFinder<UAnimMontage> AvoidAsset_F(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Dodge/Root/GhostSamurai_APose_Avoid_F_Root_Montage.GhostSamurai_APose_Avoid_F_Root_Montage'"));
		if (AvoidAsset_F.Succeeded())
			m_AvoidArray.Add(AvoidAsset_F.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> AvoidAsset_B(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Dodge/Root/GhostSamurai_APose_Avoid_B_Root_Montage.GhostSamurai_APose_Avoid_B_Root_Montage'"));
		if (AvoidAsset_B.Succeeded())
			m_AvoidArray.Add(AvoidAsset_B.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> AvoidAsset_L(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Dodge/Root/GhostSamurai_APose_Avoid_L_Root_Montage.GhostSamurai_APose_Avoid_L_Root_Montage'"));
		if (AvoidAsset_L.Succeeded())
			m_AvoidArray.Add(AvoidAsset_L.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> AvoidAsset_R(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Dodge/Root/GhostSamurai_APose_Avoid_R_Root_Montage.GhostSamurai_APose_Avoid_R_Root_Montage'"));
		if (AvoidAsset_R.Succeeded())
			m_AvoidArray.Add(AvoidAsset_R.Object);


		static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeAsset_F(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Dodge/Root/GhostSamurai_APose_Dodge_F_Root_Montage.GhostSamurai_APose_Dodge_F_Root_Montage'"));
		if (DodgeAsset_F.Succeeded())
			m_AvoidArray.Add(DodgeAsset_F.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeAsset_B(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Dodge/Root/GhostSamurai_APose_Dodge_B_Root_Montage.GhostSamurai_APose_Dodge_B_Root_Montage'"));
		if (DodgeAsset_B.Succeeded())
			m_AvoidArray.Add(DodgeAsset_B.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeAsset_L(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Dodge/Root/GhostSamurai_APose_Dodge_L_Root_Montage.GhostSamurai_APose_Dodge_L_Root_Montage'"));
		if (DodgeAsset_L.Succeeded())
			m_AvoidArray.Add(DodgeAsset_L.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeAsset_R(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Dodge/Root/GhostSamurai_APose_Dodge_R_Root_Montage.GhostSamurai_APose_Dodge_R_Root_Montage'"));
		if (DodgeAsset_R.Succeeded())
			m_AvoidArray.Add(DodgeAsset_R.Object);


		static ConstructorHelpers::FObjectFinder<UAnimMontage> SlideAsset_F(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Dodge/Root/GhostSamurai_APose_Slide_F_Root_Montage_RTI.GhostSamurai_APose_Slide_F_Root_Montage_RTI'"));
		if (SlideAsset_F.Succeeded())
			m_AvoidArray.Add(SlideAsset_F.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> SlideAsset_B(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Dodge/Root/GhostSamurai_APose_Slide_B_Root_Montage_RTI.GhostSamurai_APose_Slide_B_Root_Montage_RTI'"));
		if (SlideAsset_B.Succeeded())
			m_AvoidArray.Add(SlideAsset_B.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> SlideAsset_L(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Dodge/Root/GhostSamurai_APose_Slide_L_Root_Montage_RTI.GhostSamurai_APose_Slide_L_Root_Montage_RTI'"));
		if (SlideAsset_L.Succeeded())
			m_AvoidArray.Add(SlideAsset_L.Object);

		static ConstructorHelpers::FObjectFinder<UAnimMontage> SlideAsset_R(TEXT(
			"AnimMontage'/Game/KatanaProject/Player/animation/Dodge/Root/GhostSamurai_APose_Slide_R_Root_Montage_RTI.GhostSamurai_APose_Slide_R_Root_Montage_RTI'"));
		if (SlideAsset_R.Succeeded())
			m_AvoidArray.Add(SlideAsset_R.Object);


		// Execution Anim Montage
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ExecutionAsset_1(TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Execution/GhostSamurai_Execution01_Montage.GhostSamurai_Execution01_Montage'"));
		if (ExecutionAsset_1.Succeeded())
			m_ExecutionArray.Add(ExecutionAsset_1.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ExecutionAsset_2(TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Execution/GhostSamurai_Execution02_Montage.GhostSamurai_Execution02_Montage'"));
		if (ExecutionAsset_2.Succeeded())
			m_ExecutionArray.Add(ExecutionAsset_2.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ExecutionAsset_3(TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Execution/GhostSamurai_Execution03_Montage.GhostSamurai_Execution03_Montage'"));
		if (ExecutionAsset_3.Succeeded())
			m_ExecutionArray.Add(ExecutionAsset_3.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ExecutionAsset_4(TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Execution/GhostSamurai_Execution04_Montage.GhostSamurai_Execution04_Montage'"));
		if (ExecutionAsset_4.Succeeded())
			m_ExecutionArray.Add(ExecutionAsset_4.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ExecutionAsset_5(TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Execution/GhostSamurai_Execution05_Montage.GhostSamurai_Execution05_Montage'"));
		if (ExecutionAsset_5.Succeeded())
			m_ExecutionArray.Add(ExecutionAsset_5.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ExecutionAsset_6(TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Execution/GhostSamurai_Execution06_Montage.GhostSamurai_Execution06_Montage'"));
		if (ExecutionAsset_6.Succeeded())
			m_ExecutionArray.Add(ExecutionAsset_6.Object);
		
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ExecutionAsset_7(TEXT("AnimMontage'/Game/KatanaProject/Player/animation/Execution/GhostSamurai_Execution07_Montage.GhostSamurai_Execution07_Montage'"));
		if (ExecutionAsset_7.Succeeded())
			m_ExecutionArray.Add(ExecutionAsset_7.Object);
	}
	
	
	m_PlayerInfo.Name = TEXT("MainCharacter");
	
	m_IsEnemyLocked = false;

	m_Katana = nullptr;
	m_Sheath = nullptr;
	m_AllKatana = nullptr;

	m_Attack = false;
	m_ContinueAttack = false;
}

void AMineProtoCharacter::Attack()
{
	Super::Attack();

	// 플레이어 전방에 인살할 수 있는 Enemy가 존재하는지 찾는 함수
	// 만약 하나라도 있다면 Execution 애님 시퀀스를 재생하고 return
	if (IsAnyExecutedActor())
		return;

	if (m_Attack)
	{
		m_ContinueAttack = true;
	}
	else
	{
		// 처음 공격을 실행했다
		m_Attack = true;

		// 처음 공격을 실행했을 당시
		// ComboIndex의 랜덤값을 얻어온다
		// AttackIndex의 경우 다음 동작으로 이어질 수 있게 Index를 변화해준다
		m_ComboIndex = FMath::FRandRange(0, m_ComboAttackArray.Num() - 1);
		
		m_AnimInstance->Montage_Play(m_ComboAttackArray[m_ComboIndex].AttackArray[m_AttackIndex]);
	}
}

void AMineProtoCharacter::DoAttackCombo()
{
	Super::DoAttackCombo();

	if (!m_ContinueAttack)
	{
		// ContinueAttack이 들어오지 않았다면
		// AttackIndex를 초기화한다
		m_AttackIndex = 0;
		m_Attack = false;
	}
	else
	{
		// 연속 공격을 실행하는데 마지막 공격에서 연속 공격을 실행했다면 ComboIndex가 다시 계산될 수 있도록
		// 다양한 공격 모션이 자연스럽게 나올 수 있도록 처리
		if (m_AttackIndex == m_ComboAttackArray[m_ComboIndex].AttackArray.Num() - 1)
		{
			m_AttackIndex = 0;
			m_ComboIndex = FMath::FRandRange(0, m_ComboAttackArray.Num() - 1);
		}
		
		m_AttackIndex = (m_AttackIndex + 1) % m_ComboAttackArray[m_ComboIndex].AttackArray.Num();
		m_AnimInstance->Montage_Play(m_ComboAttackArray[m_ComboIndex].AttackArray[m_AttackIndex]);
	}
	
	// 다음번 Notify를 위해 ContinueAttack false로 변경
	m_ContinueAttack = false;
}

void AMineProtoCharacter::AttackReset()
{
	m_Attack = false;
	m_ContinueAttack = false;
	m_AttackIndex = 0;

	m_Katana->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool AMineProtoCharacter::IsAnyExecutedActor()
{
	// 가장 가까운 Enemy가 없는 경우 return
	AEnemyCharacter* EnemyCharacter = DetectClosestEnemyFunc(500.f, m_PlayerInfo.DetectAngle, false);
	if (!EnemyCharacter)
		return false;

	// Enemy가 존재했지만 해당 Enemy가 그로기 상태가 아닌 경우 return
	if (!EnemyCharacter->IsGroggy())
		return false;

	// 이미 플레이어가 Execution 몽타주를 재생하고 있다면 return
	if (m_AnimInstance->Montage_IsPlaying(m_ExecutionArray[m_ExecutionIndex]))
		return true;
	
	AttackReset();

	// Player와 Enemy의 위치를 강제로 조정한 뒤
	SetEnemyActor(EnemyCharacter);
	EnemyCharacter->StopAIController(TEXT("IsExecuted"));
	
	const FRotator ToPlayerRotation = UKismetMathLibrary::FindLookAtRotation(EnemyCharacter->GetActorLocation(), GetActorLocation());
	const FRotator YawRotation = FRotator(0, ToPlayerRotation.Yaw, 0);
	EnemyCharacter->SetActorRotation(YawRotation);

	// Execution 애니메이션 시퀀스를 재생시킨다
	m_ExecutionIndex = FMath::RandRange(0, m_ExecutionArray.Num() - 1);
	m_AnimInstance->Montage_Play(m_ExecutionArray[m_ExecutionIndex]);
	EnemyCharacter->PlayExecutedAnimMontage(m_ExecutionIndex);

	// Enemy가 존재하고 Groggy 상태인 경우
	// Execution Billboard를 비가시화
	EnemyCharacter->SetExecutionVisibility(false);
	
	return true;
}

void AMineProtoCharacter::BeginPlay()
{
	Super::BeginPlay();

	UKatanaProjectGameInstance* GameInstance = Cast<UKatanaProjectGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		const FPlayerTableInfo* PlayerInfo = GameInstance->FindPlayerInfo(m_PlayerInfo.Name);
		if (PlayerInfo)
		{
			m_PlayerInfo.Attack = PlayerInfo->Attack;
			m_PlayerInfo.Armor = PlayerInfo->Armor;
			m_PlayerInfo.HP = PlayerInfo->HP;
			m_PlayerInfo.HPMax = PlayerInfo->HPMax;
			m_PlayerInfo.SP = PlayerInfo->SP;
			m_PlayerInfo.SPMax = PlayerInfo->SPMax;
			m_PlayerInfo.SPRecoverMaxTime = PlayerInfo->SPRecoverMaxTime;
			m_PlayerInfo.Level = PlayerInfo->Level;
			m_PlayerInfo.Exp = PlayerInfo->Exp;
			m_PlayerInfo.Gold = PlayerInfo->Gold;
			m_PlayerInfo.AttackSpeed = PlayerInfo->AttackSpeed;
			m_PlayerInfo.MoveSpeed = PlayerInfo->MoveSpeed;
			m_PlayerInfo.DetectLength = PlayerInfo->DetectLength;
			m_PlayerInfo.DetectAngle = PlayerInfo->DetectAngle;
		}
	}
	
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	// 칼, 칼집 액터 생성
	m_Katana = GetWorld()->SpawnActor<AKatana>(AKatana::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, param);
	m_Sheath = GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, param);
	m_AllKatana = GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, param);
	
	// 첫 시작 시에는 카타나의 소켓 위치를 Sheath에 넣어둔다
	// 나중에 세이브 로드 생겼을 때는 나중에 처리한다
	m_Katana->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Katana"));
	m_Sheath->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Sheath"));
	m_AllKatana->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Katana"));
	
	//m_Katana->SetMesh(TEXT("StaticMesh'/Game/GhostSamurai_Bundle/GhostSamurai/Weapon/Mesh/Katana/SM_Katana01.SM_Katana01'"));
	m_Sheath->SetMesh(TEXT("StaticMesh'/Game/GhostSamurai_Bundle/GhostSamurai/Weapon/Mesh/Katana/SM_Scabbard01.SM_Scabbard01'"));
	m_AllKatana->SetMesh(TEXT("StaticMesh'/Game/GhostSamurai_Bundle/GhostSamurai/Weapon/Mesh/Katana/SM_AllKatana_1.SM_AllKatana_1'"));

	// 이후 세이브 로드 기능이 생겼을 때는
	// 저장될 당시 캐릭터 상태를 확인하여 처리할 필요가 있다
	m_Katana->SetWeaponOutput(false);
	m_Sheath->SetWeaponOutput(false);

	m_Katana->SetWeaponOwner(this);
	m_Sheath->SetWeaponOwner(this);
	m_AllKatana->SetWeaponOwner(this);

	m_Katana->GetMesh()->SetCollisionProfileName(TEXT("PlayerAttack"));
	m_Katana->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorldTimerManager().SetTimer(m_EnemyLockTimer, this, &AMineProtoCharacter::LockTimer, 0.05f, true);
}

void AMineProtoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StaminaRecoverFunc(DeltaTime);
}

void AMineProtoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMineProtoCharacter::JumpKey);
}

float AMineProtoCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (0.f == Damage || m_AnimInstance->Montage_IsPlaying(m_ExecutionArray[m_ExecutionIndex]))
		return 0.f;

	Damage = Damage - m_PlayerInfo.Armor;
	Damage = Damage < 1.f ? 1.f : Damage;	// 아머가 아무리 높아도 최소 1은 들어가게끔 설정
	//m_AnimInstance->SetPlayerHit(true);

	// 공격 당할 시 AttackIndex 초기화
	AttackReset();

	AKatanaProjectGameMode* GameMode = Cast<AKatanaProjectGameMode>(GetWorld()->GetAuthGameMode());
	const UMainHUD* MainHUD = nullptr;
	const UCharacterHUD* CharacterHUD = nullptr;
	const UStaminaHUD* StaminaHUD = nullptr;
	if (IsValid(GameMode))
	{
		MainHUD = GameMode->GetMainHUD();
		if (IsValid(MainHUD))
		{
			CharacterHUD = GameMode->GetMainHUD()->GetCharacterHUD();
			StaminaHUD = GameMode->GetMainHUD()->GetStaminaHUD();	
		}
	}
		
	
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(DamageCauser);
	if (EnemyCharacter)
	{
		if (EEnemyAnimType::HeavyAttack == EnemyCharacter->GetAnimType())
		{
			m_AnimInstance->SetDeflect(false);
			
			m_PlayerInfo.HP -= Damage * 1.2f;
			m_AnimInstance->Montage_Play(m_HitArray[0]);
			
			if (IsValid(CharacterHUD))
				CharacterHUD->SetHPPercent(m_PlayerInfo.HP / (float)m_PlayerInfo.HPMax);
			
			return Damage;
		}
	}
	

	if (m_AnimInstance->GetStateType() == EPlayerStateType::Defence)
	{
		// 플레이어를 공격한 Enemy를 바라보게 만들고
		const FVector& CauserLocation = DamageCauser->GetActorLocation();
		const FRotator ToCauserRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CauserLocation);
		const FRotator YawRotation = FRotator(0, ToCauserRotation.Yaw, 0);
		SetActorRotation(YawRotation);

		
		if (EnemyCharacter)
			SetEnemyActor(EnemyCharacter);

		// Hit 애니메이션과 Defence 애니메이션을 블렌드하여 재생
		const int DeflectIndex = FMath::FRandRange(0, 6);
		m_AnimInstance->Montage_Play(m_HitArray[0]);

		if (m_AnimInstance->GetDefenceDirType() == EDefenceDirType::DefenceL)
			m_AnimInstance->Montage_Play(m_DeflectLArray[DeflectIndex], 1.f, EMontagePlayReturnType::MontageLength, 0,
			                             false);
		else if (m_AnimInstance->GetDefenceDirType() == EDefenceDirType::DefenceR)
			m_AnimInstance->Montage_Play(m_DeflectRArray[DeflectIndex], 1.f, EMontagePlayReturnType::MontageLength, 0,
			                             false);

		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(UDeflectCameraShake::StaticClass());

		const FVector ParticleLocation = GetMesh()->GetSocketLocation(TEXT("DeflectParticle"));
		const FRotator ParticleRotation = GetMesh()->GetSocketRotation(TEXT("DeflectParticle"));

		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ANormalEffect* DeflectParticle = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(), ParticleLocation, ParticleRotation, param);
		//DeflectParticle->LoadParticle(TEXT("ParticleSystem'/Game/VFXSparkPack/Particles/VFX_ElectricBreak_mine.VFX_ElectricBreak_mine'"));
		DeflectParticle->LoadParticleAsync(TEXT("DeflectParticle"));


		m_SPRecover = false;
		m_SPRecoverTime = 0.f;
		
		m_PlayerInfo.SP -= Damage * 0.5f;
		if (IsValid(MainHUD) && IsValid(StaminaHUD))
		{
			MainHUD->SetStaminaVisibility(true);
			StaminaHUD->SetStaminaPercent(m_PlayerInfo.SP / (float)m_PlayerInfo.SPMax);
		}

		// 패링타이밍 설정
		if (m_DeflectTime < 0.5f)
		{
			// 일정 시간 이내로 막아냈다면 Parring 처리
			// FX 출력
		}
		else
		{
			// 그렇지 않다면 SP 감소
			//m_PlayerInfo.SP -= Damage;
		}
	}
	else
	{
		// 현재 Defence 상태도 아니며 누르지도 못했다면 HP 감소 및 애님 재생
		m_PlayerInfo.HP -= Damage;
		m_AnimInstance->Montage_Play(m_HitArray[0]);

		if (IsValid(CharacterHUD))
			CharacterHUD->SetHPPercent(m_PlayerInfo.HP / (float)m_PlayerInfo.HPMax);
	}
	
	if (m_PlayerInfo.HP <= 0)
	{
		PrintViewport(10.f, FColor::Red, TEXT("Player Dead"));
	}
	else
	{
		// m_AnimInstance->SetHit();
		// 
	}

	if (m_PlayerInfo.SP <= 0)
	{
		PrintViewport(5.f, FColor::Yellow, TEXT("Zero Stamina"));
	}
	
	return Damage;
}

void AMineProtoCharacter::JumpKey()
{
	//if (m_AnimInstance->GetAnimType() != EPlayerAnimType::Ground)
	//	return;

	// 카메라 Lock 중 점프 X
	if (m_AnimInstance->IsEnemyLocked())
	{
		if (!m_AnimInstance->CanBeOtherAction())
			return;

		m_AnimInstance->ChangeStateType(EPlayerStateType::Idle);
		
		AttackReset();

		// 현재 입력받고 있는 방향으로 Dodge 동작 실행
		const float MoveForwardValue = GetInputAxisValue(TEXT("MoveForward"));
		const float MoveRightValue = GetInputAxisValue(TEXT("MoveRight"));

		// 아무런 입력도 하지 않거나 S키 누르면서 Dodge할 경우 B 방향으로 Dodge
		if ((MoveForwardValue == 0.f && MoveRightValue == 0.f) ||
			MoveForwardValue == -1.f)
				m_AnimInstance->Montage_Play(m_AvoidArray[5]);
		// 방향키를 같이 누르고 있을때는...그냥 씹히도록
		else if (MoveForwardValue != 0.f && MoveRightValue != 0.f)
			return;
		else if (MoveForwardValue == 1.f)
			m_AnimInstance->Montage_Play(m_AvoidArray[4]);
		else if (MoveRightValue == -1.f)
			m_AnimInstance->Montage_Play(m_AvoidArray[6]);
		else if (MoveRightValue == 1.f)
			m_AnimInstance->Montage_Play(m_AvoidArray[7]);

		if (m_PerilousBillBoard->IsVisible())
			GetWorldSettings()->SetTimeDilation(1.f);
	}
	else
	{
		// Jump();
		//
		// m_AnimInstance->ChangeAnimType(EPlayerAnimType::Jump);
		// m_AnimInstance->SetFallRecoveryAdditive(0.f);
	}
}

void AMineProtoCharacter::FallRecovery()
{
	if (!m_FallRecoveryMontage)
		return;

	if (!m_AnimInstance->Montage_IsPlaying(m_FallRecoveryMontage))
	{
		m_AnimInstance->Montage_SetPosition(m_FallRecoveryMontage, 0.f);
		m_AnimInstance->Montage_Play(m_FallRecoveryMontage);
	}
}

void AMineProtoCharacter::CameraLockKey()
{
	Super::CameraLockKey();

	// 현재 Lock 기능 On 상태에서
	// 다시금 Lock을 호출했다면 그대로 Off 해버리고 return
	if (m_IsEnemyLocked)
	{
		ResetEnemyActor();
		return;
	}

	// 해당 적을 기억하고 있게 만든다
	// 몬스터가 없어졌는데 해당 몬스터를 가리키고 마는 댕글링 포인터를 막기 위해
	// TSharedPtr<> 언리얼 스마트 포인터를 사용한다
	// -> 스마트포인터를 쓰려면 NewObject를 통해 새로 할당해야한다
	//    하지만 나는 기존 Enemy를 알게 만들고 싶은거라서...원하는 동작이 아니다
	//	-> 그래서 m_EnemyActor를 UProperty를 설정하여 가비지 컬렉터가 수집하게 만들어
	//     가리키고 있는 Enemy가 Dangling Pointer가 되지 않도록 만들어준다
	AEnemyCharacter* EnemyCharacter = DetectClosestEnemyFunc(m_PlayerInfo.DetectLength, m_PlayerInfo.DetectAngle, true);
	SetEnemyActor(EnemyCharacter);
}

void AMineProtoCharacter::Avoid()
{
	Super::Avoid();

	AttackReset();

	// 현재 입력받고 있는 방향으로 Dodge 동작 실행
	const float MoveForwardValue = GetInputAxisValue(TEXT("MoveForward"));
	const float MoveRightValue = GetInputAxisValue(TEXT("MoveRight"));

	// 아무런 입력도 하지 않거나 S키 누르면서 Dodge할 경우 B 방향으로 Dodge
	if ((MoveForwardValue == 0.f && MoveRightValue == 0.f) ||
		MoveForwardValue == -1.f)
		m_AnimInstance->Montage_Play(m_AvoidArray[1]);
	// 방향키를 같이 누르고 있을때는...그냥 씹히도록
	else if (MoveForwardValue != 0.f && MoveRightValue != 0.f)
		return;
	else if (MoveForwardValue == 1.f)
		m_AnimInstance->Montage_Play(m_AvoidArray[0]);
	else if (MoveRightValue == -1.f)
		m_AnimInstance->Montage_Play(m_AvoidArray[2]);
	else if (MoveRightValue == 1.f)
		m_AnimInstance->Montage_Play(m_AvoidArray[3]);
}

void AMineProtoCharacter::Slide()
{
	Super::Slide();

	//PrintViewport(1.f, FColor::Red, TEXT("Avoid"));
	
	if (GetCharacterMovement()->bOrientRotationToMovement)
		GetCharacterMovement()->bOrientRotationToMovement = false;

	if (m_AnimInstance->RootMotionMode == ERootMotionMode::RootMotionFromMontagesOnly)
		m_AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
	
	AttackReset();

	// 현재 입력받고 있는 방향으로 Dodge 동작 실행
	const float MoveForwardValue = GetInputAxisValue(TEXT("MoveForward"));
	const float MoveRightValue = GetInputAxisValue(TEXT("MoveRight"));

	// 아무런 입력도 하지 않거나 S키 누르면서 Dodge할 경우 B 방향으로 Dodge
	if ((MoveForwardValue == 0.f && MoveRightValue == 0.f) || MoveForwardValue == -1.f)
		m_AnimInstance->Montage_Play(m_AvoidArray[9]);
	// 방향키를 같이 누르고 있을때는...그냥 씹히도록
	else if (MoveForwardValue != 0.f && MoveRightValue != 0.f)
		return;
	else if (MoveForwardValue == 1.f)
		m_AnimInstance->Montage_Play(m_AvoidArray[8]);
	else if (MoveRightValue == -1.f)
		m_AnimInstance->Montage_Play(m_AvoidArray[10]);
	else if (MoveRightValue == 1.f)
		m_AnimInstance->Montage_Play(m_AvoidArray[11]);
}

void AMineProtoCharacter::BeginColl()
{
	Super::BeginColl();

	m_Katana->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AMineProtoCharacter::EndColl()
{
	Super::EndColl();

	m_Katana->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMineProtoCharacter::DeflectOnKey()
{
	Super::DeflectOnKey();

	AttackReset();
}

void AMineProtoCharacter::DeflectOffKey()
{
	Super::DeflectOffKey();

	AttackReset();
}

void AMineProtoCharacter::LockTimer()
{
	// 자신이 가지고 있는 Monster의 거리가 최대 거리를 벗어났을 경우 Lock 기능을 해제해버린다
	if (!m_EnemyActor)
		return;

	const float Length = GetDistanceTo(m_EnemyActor);
	if (Length > m_PlayerInfo.DetectLength)
		ResetEnemyActor();
}

void AMineProtoCharacter::StaminaRecoverFunc(float DeltaTime)
{
	// 스테미나 회복 설정
	if (m_SPRecover)
	{
		if (m_PlayerInfo.SP == m_PlayerInfo.SPMax)
			return;
		
		if (!GetWorldTimerManager().TimerExists(m_SPRecoverTimer))
			GetWorldTimerManager().SetTimer(m_SPRecoverTimer, this, &AMineProtoCharacter::SPRecover, 0.2f, true);
		
		if (m_PlayerInfo.SP >= m_PlayerInfo.SPMax)
		{
			m_PlayerInfo.SP = m_PlayerInfo.SPMax;
			m_SPRecover = false;

			GetWorldTimerManager().ClearTimer(m_SPRecoverTimer);
		}
			
		AKatanaProjectGameMode* GameMode = Cast<AKatanaProjectGameMode>(GetWorld()->GetAuthGameMode());
		if (IsValid(GameMode))
		{
			const UMainHUD* MainHUD = GameMode->GetMainHUD();
			if (IsValid(MainHUD))
			{
				const UStaminaHUD* StaminaHUD = MainHUD->GetStaminaHUD();
				if (IsValid(StaminaHUD))
				{
					MainHUD->SetStaminaVisibility(true);
					StaminaHUD->SetStaminaPercent(m_PlayerInfo.SP / (float)m_PlayerInfo.SPMax);
				}
			}
		}
	}
	else
	{
		m_SPRecoverTime += DeltaTime;
		if (m_SPRecoverTime >= m_PlayerInfo.SPRecoverMaxTime)
		{
			m_SPRecoverTime = 0.f;
			m_SPRecover = true;
			
			AKatanaProjectGameMode* GameMode = Cast<AKatanaProjectGameMode>(GetWorld()->GetAuthGameMode());
			if (IsValid(GameMode))
			{
				const UMainHUD* MainHUD = GameMode->GetMainHUD();
				if (IsValid(MainHUD))
					MainHUD->SetStaminaVisibility(false);

				GetWorldTimerManager().ClearTimer(m_SPRecoverTimer);
			}
		}
	}
}

AEnemyCharacter* AMineProtoCharacter::DetectClosestEnemyFunc(const float DetectLength, const float DetectAngle, bool DrawDebug) const
{
	UCameraComponent* Camera = GetFollowCamera();

	const FVector PlayerLocation = GetActorLocation();
	FVector CameraForward = Camera->GetForwardVector();

	const FVector ModifyLocation = PlayerLocation - (CameraForward * 100.f);
	
	TArray<FHitResult> resultArray;
	FCollisionQueryParams param(NAME_None, false, this);

	// SweepMultiByChannel은 커다란 원기둥 모양
	// 500의 거리만큼 Sweep
	bool Detect = GetWorld()->SweepMultiByChannel(resultArray, ModifyLocation, ModifyLocation, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3, FCollisionShape::MakeSphere(DetectLength), param);

	TArray<FHitResult> CollisionArray;
	if (Detect)
	{
		for (auto& result : resultArray)
		{
			FVector vDir = result.ImpactPoint - ModifyLocation;
			vDir.Normalize();	
			CameraForward.Normalize();

			// 카메라 전방벡터와 충돌된 Enemy에서 ModifyLocation 으로의 방향벡터의 내적
			// ㄴ 내적으로 인해 두 벡터의 cos(θ) 값이 도출된다
			const float Dot = FVector::DotProduct(vDir, CameraForward);

			// Acos 함수를 이용해 θ의 값을 알아낸다 -> 해당 값은 Radian 값이다 -> Degree로 변경
			float Angle = FMath::Acos(Dot);
			Angle = FMath::RadiansToDegrees(Angle);

			// Camera가 Enemy를 찾아낼 수 있는 최대각도 45도 보다 작거나 같다면
			// 해당 Enemy를 CollisionArray에 담는다
			if (Angle <= 45.f)
				CollisionArray.Add(result);
		}
	}

	// 현재 Lock 기능이 Off 상태라서 Lock을 호출했지만
	// Enemy를 찾아내지 못했다면 그대로 return
	if (CollisionArray.Num() == 0)
		return nullptr;

	// 비교를 위한 최대 값 설정
	float MaxLengthDiff = TNumericLimits<float>::Max();

	// 그렇지 않고, Enemy를 찾아냈다면
	FHitResult FinalEnemy;
	for (auto& Enemy : CollisionArray)
	{
		// 충돌한 액터가 Enemy가 아닐 경우 continue
		if (!Cast<AEnemyCharacter>(Enemy.Actor))
			continue;
		
		// 해당 Enemy 중 플레이어의 전방벡터로부터 그리고 거리가 가장 가까운 Enemy를 선택하게 만든다
		// Enemy와 카메라 전방벡터를 빼 해당 전방벡터를 바라보는 방향벡터를 얻어낸다.
		// 얻어는 방향벡터를 플레이어의 RightVector에 사영하여 전방벡터로의 거리를 알아낸다
		// 

		// 일단 가장 가까운 위치에 있는 Enemy를 선택한다
		float LengthDiff = GetDistanceTo(Enemy.GetActor());
		if (MaxLengthDiff >= LengthDiff)
		{
			FinalEnemy = Enemy;
			MaxLengthDiff = LengthDiff;
		}
	}

	// Detect한 경우에만 Debug Cone 그리기
#ifdef ENABLE_DRAW_DEBUG
	if (FinalEnemy.GetActor() && DrawDebug)
	{
		const FColor DrawColor = CollisionArray.Num() ? FColor::Red : FColor::Green;

		DrawDebugCone(GetWorld(), ModifyLocation, Camera->GetForwardVector(), DetectLength,
		              FMath::DegreesToRadians(DetectAngle), FMath::DegreesToRadians(DetectAngle)
		              , 20, DrawColor, false, 1.f);
	}
#endif

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(FinalEnemy.GetActor());
	
	return EnemyCharacter;
}
