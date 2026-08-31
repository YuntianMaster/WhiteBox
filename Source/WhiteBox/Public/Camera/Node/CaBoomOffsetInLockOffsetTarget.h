// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/CameraNode.h"
#include "Core/CameraParameters.h"
#include "CaBoomOffsetInLockOffsetTarget.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UCaBoomOffsetInLockOffsetTarget : public UCameraNode
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "CameraVaraiable")
	FFloatCameraParameter  MinPitch;

	UPROPERTY(EditAnywhere, Category = "CameraVaraiable")
	FFloatCameraParameter  MaxPitch;

	UPROPERTY(EditAnywhere, Category = "CameraVaraiable")
	FFloatCameraParameter  CenterToEnemy;

	UPROPERTY(EditAnywhere, Category = "CameraVaraiable")
	FFloatCameraParameter  BoomZoomSpeed;

	UPROPERTY(EditAnywhere, Category = "CameraVaraiable")
	FVector3dCameraParameter  BoomOffset;

	UPROPERTY(EditAnywhere, Category = "CameraVaraiable")
	FVector3dCameraParameter  FocusTarget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float FramingSize = 0.8f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float PitchLimitMarginScale = 0.85f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float MinBoomLength = 300.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float MaxBoomLength = 1500.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float BoomHeight = 50.f;      // BoomOffset.Z
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float BoomLatera = 0.f;      // BoomOffset.Y
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float CenterWeightToEnemy = 0.5f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float PlayerHalfHeight = 90.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float EnemyHalfHeight = 90.f;

	const float ScreenMargin = 0.75f; // FramingSize


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsDebug{ false };

protected:
	virtual FCameraNodeEvaluatorPtr OnBuildEvaluator(FCameraNodeEvaluatorBuilder& Builder) const override;

};
