// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "LaunchCharacterAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API ULaunchCharacterAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Launch")
	FVector LaunchLocation;
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
