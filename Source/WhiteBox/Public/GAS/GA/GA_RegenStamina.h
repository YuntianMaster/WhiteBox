// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/GA_GameAblilityBase.h"
#include "GameplayEffectTypes.h"
#include "GA_RegenStamina.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UGA_RegenStamina : public UGA_GameAblilityBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void GA_RegenStamina();
protected:
	FTimerHandle RegenTimeHandle;
	void RegenStaminaHandle(const FOnAttributeChangeData& Data);
	UFUNCTION()
	void RegenStaminDataHandle();
	float OldValue;
	float NewValue;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TSubclassOf<UGameplayEffect> RegenEffect;
	FGameplayEffectSpecHandle RegenSpecHandle;
	FActiveGameplayEffectHandle ActiveHandle;
};
