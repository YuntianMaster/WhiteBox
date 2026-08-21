// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/Utility/BlueprintCameraNode.h"
#include "CameraAngleReset.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UCameraAngleReset : public UBlueprintCameraNodeEvaluator
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void GetAngleBetweenCameraAndTarget();
	
};
