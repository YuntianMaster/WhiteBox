// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Enum/E_Camera.h"
#include "ChangeCameraLockNotify.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UChangeCameraLockNotify : public UAnimNotify
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraLock")
	TEnumAsByte<E_LockCameraMode> CameraLockMode;

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
