// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalEffect.h"
#include "../AssetManager/AssetPathMain.h"

// Sets default values
ANormalEffect::ANormalEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));

	SetRootComponent(m_pParticleSystem);

	m_bAssetLoop = false;
}

// Called when the game starts or when spawned
void ANormalEffect::BeginPlay()
{
	Super::BeginPlay();
	
	// OnSystemFinished : ParticleSystemComponent가 가지고 있는 파티클 시스템이 재생이 모두 종료되었을 경우 호출될 함수를 지정해준다
	// 종료 시 지정된 함수를 호출해준다
	if (!m_bAssetLoop)
		m_pParticleSystem->OnSystemFinished.AddDynamic(this, &ANormalEffect::ParticleFinish);
}

// Called every frame
void ANormalEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANormalEffect::LoadParticle(const FString& _Path)
{
	UParticleSystem* pParticle = LoadObject<UParticleSystem>(GetWorld(), *_Path);
	
	if (pParticle)
		m_pParticleSystem->SetTemplate(pParticle);
		
}

void ANormalEffect::LoadParticleAsync(const FString& _Name)
{
	UAssetPathMain* pAssetPath = UAssetPathMain::StaticClass()->GetDefaultObject<UAssetPathMain>();

	const FSoftObjectPath* pPath = pAssetPath->FindParticlePath(_Name);
	if (!pPath)
		return;

	m_AsyncParticlePath = *pPath;

	// 에셋매니저에서 StreamableManager를 통해 비동기 에셋 로딩을 시도한다
	FStreamableManager& streamMgr = UAssetManager::GetStreamableManager();

	// Callback : 비동기 로딩이 끝났을 때 호출할 함수 등록 -> 내부적으로 Delegate로 되어있음
	// ㄴ 함수 지정에는 TFunction으로 들어가야함 -> FStreamableDelegate::CreateUObject()
	m_AsyncParticleLoadHandle = streamMgr.RequestAsyncLoad(m_AsyncParticlePath, 
		FStreamableDelegate::CreateUObject(this, &ANormalEffect::LoadParticleAsyncComplete));
}

void ANormalEffect::LoadParticleAsyncComplete()
{
	// 로딩이 끝나면 Handle Release
	m_AsyncParticleLoadHandle->ReleaseHandle();

	// 생성자 매개변수로 경로를 입력하면 로딩한 파티클을 얻어올 수 있도록 만들어둔다.
	TAssetPtr<UParticleSystem> NewParticle(m_AsyncParticlePath);
	if (NewParticle)
	{
		m_pParticleSystem->SetTemplate(NewParticle.Get());
		//m_pParticleSystem->OnSystemFinished.AddDynamic(this, &ANormalEffect::ParticleFinish);
	}
}

void ANormalEffect::LoadSound(const FString& _Path)
{
	USoundBase* pSound = LoadObject<USoundBase>(nullptr, *_Path);

	if (pSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), pSound, GetActorLocation());
}

void ANormalEffect::LoadSoundAsync(const FString& _Name)
{
	UAssetPathMain* pAssetPath = UAssetPathMain::StaticClass()->GetDefaultObject<UAssetPathMain>();

	const FSoftObjectPath* pPath = pAssetPath->FindSoundPath(_Name);
	if (!pPath)
	{
		LOG(TEXT("Sound Load Fail"));
		return;
	}
	
	m_AsyncSoundPath = *pPath;

	// 에셋매니저에서 StreamableManager를 통해 비동기 에셋 로딩을 시도한다
	FStreamableManager& streamMgr = UAssetManager::GetStreamableManager();

	// Callback : 비동기 로딩이 끝났을 때 호출할 함수 등록 -> 내부적으로 Delegate로 되어있음
	// ㄴ 함수 지정에는 TFunction으로 들어가야함 -> FStreamableDelegate::CreateUObject()
	m_AsyncSoundLoadHandle = streamMgr.RequestAsyncLoad(m_AsyncSoundPath,
		FStreamableDelegate::CreateUObject(this, &ANormalEffect::LoadSoundAsyncComplete));
}

void ANormalEffect::LoadSoundAsyncComplete()
{
	// 로딩이 끝나면 Handle Release
	m_AsyncSoundLoadHandle->ReleaseHandle();

	// 생성자 매개변수로 경로를 입력하면 로딩한 파티클을 얻어올 수 있도록 만들어둔다.
	TAssetPtr<USoundBase> NewSound(m_AsyncSoundPath);
	if (NewSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), NewSound.Get(), GetActorLocation());
}


void ANormalEffect::ParticleFinish(UParticleSystemComponent* _pParticle)
{
	Destroy();
}