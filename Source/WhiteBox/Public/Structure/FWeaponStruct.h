// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structure/FTraceSockets.h"
#include "AbilitySystemComponent.h"
#include "Combat/Weapon/GeneralProjectile.h"
#include "FWeaponStruct.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct WHITEBOX_API FWeaponStruct
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character")
	UAnimMontage* CHA_EquipMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character")
	UAnimMontage* CHA_UArmMontage;

	class AWeaponGeneral* WeaponGeneral;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	USkeletalMesh* WeaponMesh;	
	
	USkeletalMeshComponent* WeaponComp;
	class UNiagaraComponent* NiagaraComp;
	AActor* WeaponActor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	FName HandSocketName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	FName BackSocketName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	TSubclassOf<UAnimInstance> WeaponABP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	FTraceSockets TraceSockets;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	UAnimMontage* WPN_EquipMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	UAnimMontage* WPN_UArmMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	TArray<TSubclassOf<UGameplayAbility>> WeaponAbilities;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	bool bAutoVisable{ true };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	TSubclassOf<AGeneralProjectile> WeaponProjectile;
};
