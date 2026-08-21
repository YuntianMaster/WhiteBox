// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/GA_GameAblilityBase.h"
#include "GA_PlayMontage.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UGA_PlayMontage : public UGA_GameAblilityBase
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MontageSettings")
	UAnimMontage* AnimMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MontageSettings")
	float PlayRate{ 1.0f };

protected:
	UFUNCTION(BlueprintCallable)
	void PlayMontageTask();

	UFUNCTION()
	void OnMontageEndHandle();
};
