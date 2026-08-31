// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_ThrowProjectile.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Enemy/EnemyCharacter.h"
#include "Combat/Weapon/GeneralProjectile_AI.h"
#include "Combat/Weapon/WeaponGeneral.h"
#include "Combat/WeaponSystemComp.h"



void UGA_ThrowProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UAbilityTask_WaitGameplayEvent* OnShowWeaponEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag("Event.Throw.ShowWeapon"),
		nullptr,
		true,
		true
	);
	OnShowWeaponEvent->EventReceived.AddDynamic(this, &UGA_ThrowProjectile::ShowWeaponHandle);
	OnShowWeaponEvent->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* OnRemoveWeaponEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag("Event.Throw.RemoveWeapon"),
		nullptr,
		true,
		true

	);
	OnRemoveWeaponEvent->EventReceived.AddDynamic(this, &UGA_ThrowProjectile::RemoveWeaponHandle);
	OnRemoveWeaponEvent->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* OnThrowWeaponEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag("Event.Throw.ThrowPrimeWeapon"),
		nullptr,
		true,
		true

	);
	OnThrowWeaponEvent->EventReceived.AddDynamic(this, &UGA_ThrowProjectile::ThrowPrimeWeaponHandle);
	OnThrowWeaponEvent->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* OnThrowSecondWeaponEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag("Event.Throw.ThrowSecondWeapon"),
		nullptr,
		true,
		true

	);
	OnThrowSecondWeaponEvent->EventReceived.AddDynamic(this, &UGA_ThrowProjectile::ThrowSecondWeaponHandle);
	OnThrowSecondWeaponEvent->ReadyForActivation();


}


//动画事件绑定
void UGA_ThrowProjectile::RemoveWeaponHandle(FGameplayEventData Payload)
{
	UE_LOG(LogTemp,Warning, TEXT("RemoveWeaponHandle: %s!!"), *Payload.Instigator->GetName());
	UWeaponSystemComp* WSC = Payload.Instigator->GetComponentByClass<UWeaponSystemComp>();
	if (!WSC)
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveWeaponHandle: NO WSC!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("ECurrent Weapon: %s"), *UEnum::GetValueAsString(WSC->ECurrentWeapons));
	TArray<AWeaponGeneral*> Weapons = WSC->GetWeaponByEWeapon(WSC->ECurrentWeapons);
	for (AWeaponGeneral* Weapon : Weapons) {

		Weapon->WeaponMeshComp->SetVisibility(false);

	}
	
}

void UGA_ThrowProjectile::ThrowPrimeWeaponHandle(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("ThrowWeaponHandle: %s!!"), *Payload.Instigator->GetName());
	UWeaponSystemComp* WSC = Payload.Instigator->GetComponentByClass<UWeaponSystemComp>();
	if (!WSC)
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveWeaponHandle: NO WSC!"));
		return;
	}
	TArray<AWeaponGeneral*> Weapons = WSC->GetWeaponByEWeapon(WSC->ECurrentWeapons);
	FWeaponStruct WeaponStuctData = Weapons[0]->WeaponStructData;

	FTransform WeaponSpawnTran = EnemyCharRef->GetComponentByClass<USkeletalMeshComponent>()->GetSocketTransform(WeaponStuctData.HandSocketName);
	AGeneralProjectile_AI* ProjectileActor = Cast<AGeneralProjectile_AI>(GetWorld()->SpawnActor(WeaponStuctData.WeaponProjectile, &WeaponSpawnTran));
	ProjectileActor->Montage_ID = MontageInstanceID;
	ProjectileActor->SetInstigator(EnemyCharRef);
	ProjectileActor->bIsUp = bPrimeUp;
	ProjectileActor->bIsRight = bPrimeRight;
	ProjectileActor->Fire(1.f);
	
}

void UGA_ThrowProjectile::ThrowSecondWeaponHandle(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("ThrowWeaponHandle: %s!!"), *Payload.Instigator->GetName());
	UWeaponSystemComp* WSC = Payload.Instigator->GetComponentByClass<UWeaponSystemComp>();
	if (!WSC)
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveWeaponHandle: NO WSC!"));
		return;
	}
	TArray<AWeaponGeneral*> Weapons = WSC->GetWeaponByEWeapon(WSC->ECurrentWeapons);
	FWeaponStruct WeaponStuctData = Weapons[0]->WeaponStructData;

	FTransform WeaponSpawnTran = EnemyCharRef->GetComponentByClass<USkeletalMeshComponent>()->GetSocketTransform(WeaponStuctData.HandSocketName);
	AGeneralProjectile_AI* ProjectileActor = Cast<AGeneralProjectile_AI>(GetWorld()->SpawnActor(WeaponStuctData.WeaponProjectile, &WeaponSpawnTran));
	ProjectileActor->Montage_ID = MontageInstanceID;
	ProjectileActor->SetInstigator(EnemyCharRef);
	ProjectileActor->bIsUp = bSecondUp;
	ProjectileActor->bIsRight = bSecondRight;
	ProjectileActor->Fire(1.f);
}

void UGA_ThrowProjectile::ShowWeaponHandle(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("ShowWeaponHandle: %s!!"), *Payload.Instigator->GetName());
	UWeaponSystemComp* WSC = Payload.Instigator->GetComponentByClass<UWeaponSystemComp>();
	if (!WSC)
	{
		UE_LOG(LogTemp, Warning, TEXT("ThrowWeaponHandle: NO WSC!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("ECurrent Weapon: %s"), *UEnum::GetValueAsString(WSC->ECurrentWeapons));
	TArray<AWeaponGeneral*> Weapons = WSC->GetWeaponByEWeapon(WSC->ECurrentWeapons);
	for (AWeaponGeneral* Weapon : Weapons) {

		Weapon->WeaponMeshComp->SetVisibility(true);

	}
}

//复写播放Montage