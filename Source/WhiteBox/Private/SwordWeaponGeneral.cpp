// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordWeaponGeneral.h"
#include "Player/PlayerCharacter.h"

ASwordWeaponGeneral::ASwordWeaponGeneral()
{
	WeaponName = EWeapons::GeneralSword;
	HandTargetSocket = "katana_r";
	BackTargetSocket = "katana_Targer01";
}

void ASwordWeaponGeneral::BlockFuction()
{
	if (!PlayerRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharRef is null!"));
		return;
	}

	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimInstance is null!"));
		return;
	}

	if (!BlockMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("BlockMontage is null!"));
		return;
	}
	Cast<APlayerCharacter>(PlayerRef)->PlayerStats = EPlayerStates::SwordDFence;
}

void ASwordWeaponGeneral::CancelBlockFuction()
{
	Cast<APlayerCharacter>(PlayerRef)->PlayerStats = EPlayerStates::CharacterNoneStats;
}



