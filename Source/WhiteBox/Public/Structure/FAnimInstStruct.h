// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FAnimInstStruct.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct WHITEBOX_API FAnimInstStruct
{
	GENERATED_BODY();
	UPROPERTY(BlueprintReadWrite)
	FVector BoneWarpWorldLoc = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite)
	FRotator BoneWarpWorldRot = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadWrite)
	bool BoneWarpActive = false;
};
