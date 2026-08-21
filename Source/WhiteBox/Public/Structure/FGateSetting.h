// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGateSetting.generated.h"

USTRUCT(BlueprintType)
struct WHITEBOX_API FGateSetting
{
public:
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxWalkSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxAcceleration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BrakingDeceleration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BrakingFrictionFactor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BrakingFriction;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool UseSeperateBrakingFriction;


};