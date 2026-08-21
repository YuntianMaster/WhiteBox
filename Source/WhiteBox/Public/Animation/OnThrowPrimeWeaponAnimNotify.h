// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "OnThrowPrimeWeaponAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UOnThrowPrimeWeaponAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
