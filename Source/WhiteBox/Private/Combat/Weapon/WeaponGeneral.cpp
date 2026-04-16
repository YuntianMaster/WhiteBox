// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapon/WeaponGeneral.h"
#include "Combat/WeaponSystemComp.h"
#include "Animation/PlayerAnimInstance.h"
#include "Interface/MainPlayerInterface.h"
#include "Combat/CombatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/StatsComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "Structure/FWeaponStruct.h"
#include "Player/PlayerCharacter.h"
#include "Combat/LockComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
AWeaponGeneral::AWeaponGeneral()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void AWeaponGeneral::BeginPlay()
{
	Super::BeginPlay();
	PlayerRef = Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	AnimInstance = Cast<UPlayerAnimInstance>(PlayerRef->GetMesh()->GetAnimInstance());
	StatsComp = PlayerRef->GetComponentByClass<UStatsComponent>();
	if (PlayerRef->Implements<UMainPlayerInterface>()) {
		IplayerRef = Cast<IMainPlayerInterface>(PlayerRef);
	}
	MeshComp = GetComponentByClass<USkeletalMeshComponent>();

}
void AWeaponGeneral::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

float AWeaponGeneral::WeaponAttack(UAnimMontage* AttackMontage)
{
	if (IplayerRef && !IplayerRef->HasEnoughStamina(AttackStaminaCost)) return 0.f;
	StatsComp->ReduceStamina(AttackStaminaCost);
	return PlayerRef->PlayAnimMontage(AttackMontage);
}

void AWeaponGeneral::EquipWeapon()
{
	if (AnimInstance->GetCurrentActiveMontage())
		return;
	if(PlayerRef)
	{
		PlayerRef->PlayAnimMontage(EquipMontage);
	}
	
	
}

void AWeaponGeneral::UArmWeapon()
{
	if (AnimInstance->GetCurrentActiveMontage())
		return;
	
	if (PlayerRef)
	{
		PlayerRef->PlayAnimMontage(UArmMontage);
	}
	
}

void AWeaponGeneral::ChargingWeapon() {

	charingTime = 0.f;
	GetWorld()->GetTimerManager().SetTimer(
		FCharingTimerHandle,
		this,
		&AWeaponGeneral::ChargingTimeHandler,
		GetWorld()->DeltaTimeSeconds,
		true
	);

}
void AWeaponGeneral::ChargingTimeHandler() {

	if (!IplayerRef->HasEnoughStamina(CharingStaminaCost))
	{
		FReleaseCharingDelegate.AddDynamic(this, &AWeaponGeneral::CancelBlockFuction);
		ReleaseCharing();
		return;
	}
	FReleaseCharingDelegate.Clear();
	charingTime += GetWorld()->DeltaTimeSeconds;
	charingTime = UKismetMathLibrary::FClamp(charingTime, 0.f, MaxCharingTime);
	StatsComp->ReduceStamina(CharingStaminaCost);
	

}

void AWeaponGeneral::ReleaseCharing()
{
	UE_LOG(LogTemp, Warning, TEXT("Release!!!! !!!"));
	GetWorld()->GetTimerManager().ClearTimer(FCharingTimerHandle);
}










