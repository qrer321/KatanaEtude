// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "../Player/PrototypeCharacter.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{
	m_AnimType = EEnemyAnimType::Idle;

	m_Hit = 0.f;
	m_Groggy = false;
	m_GroggyElapsedTime = 0.f;
	m_GroggyMaxTime = 3.f;

	m_Executed = false;
}

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UEnemyAnimInstance::NativeUpdateAnimation(float FDeltaSeconds)
{
	Super::NativeUpdateAnimation(FDeltaSeconds);

}

void UEnemyAnimInstance::SetHit()
{
	m_Hit = 0.4f;
}

void UEnemyAnimInstance::SetGroggy(bool Groggy)
{
	m_Groggy = Groggy;
	if (m_Groggy)
	{
		if (!GetWorld()->GetTimerManager().TimerExists(m_GroggyTimer))
			GetWorld()->GetTimerManager().SetTimer(m_GroggyTimer, this, &UEnemyAnimInstance::GroggyTimer, 0.1f, true);
	}
	else
		GetWorld()->GetTimerManager().ClearTimer(m_GroggyTimer);
}

void UEnemyAnimInstance::GroggyTimer()
{
	// SP 회복 불가
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(TryGetPawnOwner());
	if (!EnemyCharacter)
		return;
	
	EnemyCharacter->SetSPRecover(false);
	

	m_GroggyElapsedTime += 0.1f;
	PrintViewport(0.1f, FColor::Yellow, FString::Printf(TEXT("Elapsed Time : %f"), m_GroggyElapsedTime));
	
	if (m_GroggyElapsedTime >= m_GroggyMaxTime)
	{	
		// 그로기 상태가 최대 시간을 넘었다면
		// 타이머 핸들러 초기화
		m_GroggyElapsedTime = 0.f;
		GetWorld()->GetTimerManager().ClearTimer(m_GroggyTimer);

		// 그로기 상태 초기화
		m_Groggy = false;
		m_Hit = 0.f;
		AEnemyAIController* AIController = Cast<AEnemyAIController>(EnemyCharacter->GetController());
		if (AIController)
			AIController->GetBlackboardComponent()->SetValueAsFloat(TEXT("Hit"), 0.f);

		// SP 최대치 설정
		EnemyCharacter->SetSPToMax();

		// 인살 빌보드 컴포넌트 비가시화
		EnemyCharacter->SetExecutionVisibility(false);
	}
}

void UEnemyAnimInstance::AnimNotify_HitEnd()
{
	PrintViewport(2.f, FColor::Blue, TEXT("HitEnd"));
	m_Hit = 0.f;

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(TryGetPawnOwner());
	if (EnemyCharacter)
	{
		AEnemyAIController* AIController = Cast<AEnemyAIController>(EnemyCharacter->GetController());
		if (AIController)
			AIController->GetBlackboardComponent()->SetValueAsFloat(TEXT("Hit"), 0.f);
	}
}

void UEnemyAnimInstance::AnimNotify_CheckNextCombo()
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(TryGetPawnOwner());
	if (EnemyCharacter)
	{
		EnemyCharacter->SetAttackEnd();
	}
}

void UEnemyAnimInstance::AnimNotify_ComboCheck()
{
	// 연속으로 공격하는걸 막도록 설정
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(TryGetPawnOwner());
	if (EnemyCharacter)
	{
		EnemyCharacter->SetAttackCoolTime();
	}
}

void UEnemyAnimInstance::AnimNotify_OnCollStart_Attack()
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(TryGetPawnOwner());
	if (EnemyCharacter)
	{
		EnemyCharacter->BeginColl();
	}
}

void UEnemyAnimInstance::AnimNotify_OnCollEnd_Attack()
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(TryGetPawnOwner());
	if (EnemyCharacter)
	{
		EnemyCharacter->EndColl();
	}
}

void UEnemyAnimInstance::AnimNotify_Perilous_Start()
{
	APrototypeCharacter* PlayerCharacter = Cast<APrototypeCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (PlayerCharacter)
	{
		int32 PerilousCount = PlayerCharacter->GetPerilousCount();
		PlayerCharacter->SetPerilousCount(++PerilousCount);
		
		PlayerCharacter->SetPerilousVisible(true);
		SetPerilousVisible(true);
	}
}

void UEnemyAnimInstance::AnimNotify_Perilous_End()
{
	APrototypeCharacter* PlayerCharacter = Cast<APrototypeCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (PlayerCharacter)
	{
		SetPerilousVisible(false);
		
		int32 PerilousCount = PlayerCharacter->GetPerilousCount();
		if (PerilousCount < 1)
		{
			PlayerCharacter->SetPerilousCount(0);
			return;
		}
			
		if (PerilousCount > 1)
			PlayerCharacter->SetPerilousCount(--PerilousCount);
		else if (PerilousCount == 1)
		{
			PlayerCharacter->SetPerilousCount(--PerilousCount);
			PlayerCharacter->SetPerilousVisible(false);
		}
	}
}

void UEnemyAnimInstance::AnimNotify_IsExecuted()
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(TryGetPawnOwner());
	if (EnemyCharacter)
		EnemyCharacter->EnemyExecuted();
}
