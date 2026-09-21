// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/GA_WarpMontageAttack.h"
#include "GA_HitFeedback.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UGA_HitFeedback : public UGA_WarpMontageAttack
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "HitStop")
	bool bIsHitStop{ false };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bIsHitStop"), Category = "HitStop")
	float HitStopScale = 0.2f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bIsHitStop"), Category = "HitStop")
	float HitStopTime = 0.15f;
	FTimerHandle HitStopHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag GamePlayCueTag;

protected:
	void OnHitStopHandler();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


};
