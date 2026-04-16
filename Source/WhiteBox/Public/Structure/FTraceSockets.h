// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FTraceSockets.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct WHITEBOX_API FTraceSockets
{
	
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Start;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName End;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Rotation;
};
