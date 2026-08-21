// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/CameraVariableReferences.h"
#include "GameFramework/BlueprintCameraVariableTable.h"
#include "GameplayTagContainer.h"
#include "CamerManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraChangeSignature, FGameplayTag, Tag);

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
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag CurrentCameraTag;
	UPROPERTY(BlueprintAssignable)
	FOnCameraChangeSignature FOnCameraChangeDelegate;
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

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	//float FramingSize = 0.8f;     // 包围盒占屏比例，1=贴边
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	//float MinBoomLength = 300.f;  // 正数：臂长（世界距离）
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	//float MaxBoomLength = 1500.f;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	//float BoomHeight = 50.f;      // BoomOffset.Z
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	//float BoomLateral = 0.f;      // BoomOffset.Y
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	//float CenterWeightToEnemy = 0.5f;
	//
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	//float PlayerHalfHeight = 90.f;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	//float EnemyHalfHeight = 90.f;



	const float ScreenMargin = 0.75f; // 或直接用 FramingSize
	

protected:
	virtual void BeginPlay() override;
	

	
};
