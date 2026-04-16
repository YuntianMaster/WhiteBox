// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapon/WeaponGeneral.h"
#include "Enum/EBowStats.h"
#include "WeanponBow.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API AWeanponBow : public AWeaponGeneral
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EBowStats> BowStats{ EBowStats::BowIdle };
};
