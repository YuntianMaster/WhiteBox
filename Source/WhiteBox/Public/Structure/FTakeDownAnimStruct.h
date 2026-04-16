// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FTakeDownAnimStruct.generated.h"

USTRUCT(BlueprintType)
struct WHITEBOX_API FTakeDownAnimStruct
{
	GENERATED_BODY();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UAnimMontage* Attack;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* UnderAttack;
};
