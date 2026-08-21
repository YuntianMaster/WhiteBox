// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapon/WeaponGeneral.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"



// Sets default values for this component's properties
AWeaponGeneral::AWeaponGeneral()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;
	WeaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKM_Comp"));
	WeaponNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara_Comp"));
	USceneComponent* DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);
	WeaponMeshComp->SetupAttachment(DefaultSceneRoot);
	WeaponNiagaraComp->bAutoActivate = false;
}


// Called when the game starts
void AWeaponGeneral::BeginPlay()
{
	
	Super::BeginPlay();

	if(GetInstigator())
		PlayerRef = Cast<ACharacter>(GetInstigator());

	//UE_LOG(LogTemp, Warning, TEXT("ImplementsInterface = %d"),
	//	GetClass()->ImplementsInterface(UNiagaraParticleCallbackHandler::StaticClass()));


	
}
void AWeaponGeneral::InitWeapon(FWeaponStruct WeaponStruct)
{
	if (!PlayerRef)
	{
		if (APawn* InstigatorPawn = GetInstigator())
		{
			PlayerRef = Cast<ACharacter>(InstigatorPawn);
		}
		if (!PlayerRef)
		{
			PlayerRef = Cast<ACharacter>(GetOwner());
		}
	}

	WeaponMeshComp->SetSkeletalMesh(WeaponStruct.WeaponMesh);
	WeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (!WeaponStruct.bAutoVisable)
		WeaponMeshComp->SetVisibility(false);
	FAttachmentTransformRules AttachRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true
	);

	if (PlayerRef && PlayerRef->GetMesh())
	{
		AttachToComponent(PlayerRef->GetMesh(), AttachRules, WeaponStruct.BackSocketName);
	}

	WeaponStruct.WeaponActor = this;
	WeaponStruct.WeaponComp = WeaponMeshComp;
	UE_LOG(LogTemp, Warning, TEXT("BackSocketName: %s"), *WeaponStruct.BackSocketName.ToString());

	if (WeaponStruct.WeaponABP)
	{
		WeaponMeshComp->SetAnimInstanceClass(WeaponStruct.WeaponABP);
	}
	if (NiragaraAttachSocketName != NAME_None)
		WeaponNiagaraComp->SetupAttachment(WeaponMeshComp, NiragaraAttachSocketName);
	WeaponStructData = WeaponStruct;
	SetupNiagaraFromStruct(WeaponStruct);
}

void AWeaponGeneral::RegisterDataInterface()
{
	WeaponNiagaraComp->SetVariableObject(TEXT("User.Data"), this);
}

void AWeaponGeneral::SetupNiagaraFromStruct(const FWeaponStruct& WeaponStruct)
{
	if (!WeaponNiagaraComp)
	{
		return;
	}

	if (IsValid(WeaponStruct.NiagaraComp) && WeaponStruct.NiagaraComp->GetAsset())
	{
		WeaponNiagaraComp->SetAsset(WeaponStruct.NiagaraComp->GetAsset());
	}

	if (!WeaponNiagaraComp->GetAsset())
	{
		UE_LOG(LogTemp, Error, TEXT("SetupNiagaraFromStruct: WeaponNiagaraComp has no Niagara System asset."));
		return;
	}

	WeaponNiagaraComp->AttachToComponent(
		WeaponMeshComp,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		NiragaraAttachSocketName);

	WeaponNiagaraComp->SetVariableObject(TEXT("User.Data"), this);

	if (WeaponNiagaraComp->IsActive())
	{
		WeaponNiagaraComp->ReinitializeSystem();
	}
	else
	{
		WeaponNiagaraComp->Activate(true);
	}
}
void AWeaponGeneral::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset)
{
	UE_LOG(LogTemp, Warning, TEXT("ReceiveParticleData called with %d particles"), Data.Num());
}
void AWeaponGeneral::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}













