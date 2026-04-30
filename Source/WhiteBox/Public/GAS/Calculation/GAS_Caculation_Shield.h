// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "GAS_Caculation_Shield.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UGAS_Caculation_Shield : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UGAS_Caculation_Shield();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	FGameplayEffectAttributeCaptureDefinition StrengthDef;
	FGameplayEffectAttributeCaptureDefinition ArmorDef;
	
};
