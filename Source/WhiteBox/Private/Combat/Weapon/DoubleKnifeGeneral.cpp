// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapon/DoubleKnifeGeneral.h"
#include "combat/Weapon/KnifeGeneralProjectile.h"
#include "Player/PlayerCharacter.h"

ADoubleKnifeGeneral::ADoubleKnifeGeneral()
{
	WeaponName = EWeapons::GeneralDoubleKnife;
	HandTargetSocket = "hand_r";
	BackTargetSocket = "hand_r";
}

float ADoubleKnifeGeneral::WeaponAttack(UAnimMontage* AttackMontage)
{
	
	
	APlayerCharacter* PlayerCharacterRef = Cast<APlayerCharacter>(PlayerRef);
	if (!PlayerCharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player class is null!"));
		return 0.0f;
	}


	AKnifeGeneralProjectile* ProjectileRef =  
		GetWorld()->SpawnActor<AKnifeGeneralProjectile>(ProjectileActorRef, PlayerCharacterRef->GetTransform());

	if (PlayerCharacterRef->LockComp->TargetWeaknessActors.Num() == 0) {

		UE_LOG(LogTemp, Warning, TEXT("No WeaknessTarget"));
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No WeaknessTarget num: %d"), PlayerCharacterRef->LockComp->TargetWeaknessActors.Num());
		AActor* target = PlayerCharacterRef->LockComp->TargetWeaknessActors[0];
		ProjectileRef->SetTarget(target);
	}
	
	
	ProjectileRef->Fire(1.f);

	float AttackDuration = Super::WeaponAttack(AttackMontage);
	return AttackDuration;
}


