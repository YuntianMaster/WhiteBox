// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/CameraNode.h"
#include "SideFocusCameraNode.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API USideFocusCameraNode : public UCameraNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Camera|SideFocus")
	float Distance = 300.f;
	UPROPERTY(EditAnywhere, Category = "Camera|SideFocus")
	FRotator OffsetRotation;
	UPROPERTY(EditAnywhere, Category = "Camera|SideFocus")
	FVector OffsetLocation;

protected:
	// UCameraNode interface.
	virtual FCameraNodeEvaluatorPtr OnBuildEvaluator(FCameraNodeEvaluatorBuilder& Builder) const override;
};
