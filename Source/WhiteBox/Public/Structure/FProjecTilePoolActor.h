// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FProjecTilePoolActor.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct WHITEBOX_API FProjecTilePoolActor
{
	GENERATED_BODY();
	class USplineMeshComponent* SplineMesh;
	bool isActived;
};
