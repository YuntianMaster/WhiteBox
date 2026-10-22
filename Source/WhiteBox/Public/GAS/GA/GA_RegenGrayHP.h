// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/GA_GameAblilityBase.h"
#include "GA_RegenGrayHP.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UGA_RegenGrayHP : public UGA_GameAblilityBase
{
	GENERATED_BODY()
public:

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	void RegenGrayHPHandle(const FOnAttributeChangeData& Data);
	UFUNCTION()
	void RegenGrayHPDataHandle();
	
	FGameplayEffectSpecHandle RegenGrayHPEffectHandle;
	FActiveGameplayEffectHandle ActiveRegenGrayHPEffectHandle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> RegenGrayHPEffect;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float DelayRecoveyGrayHPTime = 5.f;
};
