// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/GA_GameAblilityBase.h"
#include "GA_TakeHit.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UGA_TakeHit : public UGA_GameAblilityBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void OnTakeHitHandle(const FGameplayEventData& EventData);
	
};
