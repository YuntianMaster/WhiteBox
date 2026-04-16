// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapon/WeaponGeneral.h"
#include "SwordWeaponGeneral.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API ASwordWeaponGeneral : public AWeaponGeneral
{
	GENERATED_BODY()
public:
	ASwordWeaponGeneral();
	virtual void BlockFuction()override;
	virtual void CancelBlockFuction()override;
};
