// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/GA_HitFeedback.h"
#include "GA_Block.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UGA_Block : public UGA_HitFeedback
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void BlockHandler(FGameplayEventData Payload);
};
