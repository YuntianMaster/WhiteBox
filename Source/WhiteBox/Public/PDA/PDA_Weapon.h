// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PDA_Weapon.generated.h"


/**
 * 
 */
UCLASS()
class WHITEBOX_API UPDA_Weapon : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	TEnumAsByte<enum EWeapons> WeaponName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Weapon")
	TArray<struct FWeaponStruct> WeaponStructs;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharaterAnimation")
	UAnimMontage* EquipAnim;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharaterAnimation")
	UAnimMontage* UnarmAnim;

};
