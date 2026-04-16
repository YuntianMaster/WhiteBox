// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FComboAttackMontages.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct WHITEBOX_API FComboAttackMontages
{
public:
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<UAnimMontage*> CombAttackMontages;
	
};
