// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/CameraVariableReferences.h"
#include "GameFramework/BlueprintCameraVariableTable.h"
#include "GameplayTagContainer.h"
#include "Enum/E_Camera.h"
#include "Core/CameraEvaluationContext.h"
#include "Core/CameraNodeEvaluator.h"
#include "Structure/FCameraArmStruct.h"
#include "CamerManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraChangeSignature, FGameplayTag, Tag);
DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnCameraLockModeChangeSignature, 
	E_LockCameraMode /*CameraLockMode*/
	);

DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnCameraBoomArmValueChangeSignature,
	FCameraArmStruct /*ArmStruct*/
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHITEBOX_API UCamerManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCamerManagerComponent();
	
	/** 切换 Rig 前写入 Evaluation Context 的 Initial Pose，供 Entry Transition 的 Context Yaw Pitch 读取。 */
	void SetInitialCameraPose();
	//UFUNCTION(BlueprintCallable)
	//void CaculateBoomArmForLockingRig();

	UFUNCTION(BlueprintCallable)
	void CameraChangeHandle(UPARAM(meta = (Categories = "Camera")) FGameplayTag CameraTag);
	UFUNCTION(BlueprintCallable)
	void CameraModeChangeHandle(UPARAM(meta = (Categories = "Camera")) E_LockCameraMode CameraLockMode);
	UFUNCTION(BlueprintCallable)
	void CameraBoomArmValueChangeHandle(FCameraArmStruct ArmStruct);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag CurrentCameraTag;
	UPROPERTY(BlueprintAssignable)
	FOnCameraChangeSignature FOnCameraChangeDelegate;
	FOnCameraLockModeChangeSignature FOnCameraLockModeChangeDelegate;
	FOnCameraBoomArmValueChangeSignature FOnCameraBoomArmValueChangeDelegate;
	UPROPERTY(BlueprintReadWrite)
	FVector BoomOffset;


	FVector PlayerLoc;
	FVector EnemyLoc;
	float VerticalFOVDeg;
	float Aspect;

	/** 从 Camera Variable Collection 中选择，不要用普通资产槽。 */
	UPROPERTY(EditAnywhere, Category = "Camera")
	FFloatCameraVariableReference MaxPitchVariable;
	UPROPERTY(EditAnywhere, Category = "Camera")
	FFloatCameraVariableReference MinPitchVariable;

	FBlueprintCameraVariableTable Table;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<E_LockCameraMode> CurrentCameraLockMode{E_LockCameraMode::LockTarget};
	
	



	const float ScreenMargin = 0.75f; // 或直接用 FramingSize
	

protected:
	virtual void BeginPlay() override;
	

	
};
