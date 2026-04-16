// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/WeaknessActor.h"
#include "FWeaknessStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct WHITEBOX_API FWeaknessStruct
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AWeaknessActor> WeaknessActor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName SocketName;
};
