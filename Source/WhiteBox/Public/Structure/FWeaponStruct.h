// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structure/FTraceSockets.h"
#include "AbilitySystemComponent.h"
#include "FWeaponStruct.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct WHITEBOX_API FWeaponStruct
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USkeletalMesh* WeaponMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USkeletalMeshComponent* WeaponComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName HandSocketName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName BackSocketName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UAnimInstance> WeaponABP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTraceSockets TraceSockets;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<UGameplayAbility>> WeaponAbilities;
};
