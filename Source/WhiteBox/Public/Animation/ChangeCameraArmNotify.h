// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structure/FCameraArmStruct.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ChangeCameraArmNotify.generated.h"


/**
 * 
 */
UCLASS()
class WHITEBOX_API UChangeCameraArmNotify : public UAnimNotify
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoomArm")
	FCameraArmStruct ArmValue;

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
