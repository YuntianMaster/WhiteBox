// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapon/WeaponGeneral.h"
#include "DoubleKnifeGeneral.generated.h"

class AKnifeGeneralProjectile;
/**
 * 
 */
UCLASS()
class WHITEBOX_API ADoubleKnifeGeneral : public AWeaponGeneral
{
	GENERATED_BODY()

public:
	ADoubleKnifeGeneral();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TSubclassOf<AKnifeGeneralProjectile> ProjectileActorRef;
	virtual float WeaponAttack(UAnimMontage* AttackMontage) override;
	
};
