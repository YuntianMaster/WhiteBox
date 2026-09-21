// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "QTECheckAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UQTECheckAnimNotify : public UAnimNotify_PlayMontageNotify
{
	GENERATED_BODY()

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float QTE_Time;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName BoneName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SlowRate{ 0.2f };


};
