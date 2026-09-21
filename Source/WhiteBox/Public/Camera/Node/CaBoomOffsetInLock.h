// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/CameraNode.h"
#include "Core/CameraParameters.h"
#include "Structure/FCameraArmStruct.h"
#include "CaBoomOffsetInLock.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UCaBoomOffsetInLock : public UCameraNode
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
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	//float MinBoomLength = 300.f;  
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	//float MaxBoomLength = 1500.f;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	//float BoomHeight = 50.f;      // BoomOffset.Z
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	//float BoomLatera = 0.f;      // BoomOffset.Y
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float CenterWeightToEnemy = 0.5f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float PlayerHalfHeight = 90.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float EnemyHalfHeight = 90.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float PlayerHalfWeight = 90.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float EnemyHalfWeight = 90.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	FCameraArmStruct InitalArmValue;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float CameraRotMoveSpeed{ 700.f };


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DeadZone")
	float DeadHalf = 0.2f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DeadZone")
	float DeadZoneRecoverySpeed = 1000.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SoftZone")
	float SoftHalf = 0.3f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SoftZone")
	float SoftZoneRecoverySpeed = 1800.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SoftZone")
	bool bIsDebug;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DeadZone")
	float UnlockRadius = 0.005f;           
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DeadZone")
	float W0 = 10.f;                      

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DeadZone")
	FVector2D ScreenCenter{ 0.5f,0.5f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DeadZone")
	float DeadZoneDistanceThreshold{ 100.f };

	const float ScreenMargin = 0.75f; // ��ֱ���� FramingSize

protected:
	virtual FCameraNodeEvaluatorPtr OnBuildEvaluator(FCameraNodeEvaluatorBuilder& Builder) const override;
};
