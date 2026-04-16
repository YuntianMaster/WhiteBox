// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_RollAbility.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UGA_RollAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ROLL")
	TArray<UAnimMontage*> Roll_F;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ROLL")
	TArray<UAnimMontage*> Roll_R;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ROLL")
	TArray<UAnimMontage*> Roll_B;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ROLL")
	TArray<UAnimMontage*> Roll_L;
	UFUNCTION(BlueprintCallable)
	void GA_Roll();
	UFUNCTION()
	void GA_RollActive();
	UFUNCTION()
	void GA_RollEnd();
	FTimerHandle RollFinishTimeHandler;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ROLL")
	TSubclassOf<UGameplayEffect> GE_Rolling;
protected:
	ACharacter* CharacterRef{};
	int RollIndex{ 0 };
	class UAbilityTask_WaitDelay* DelayEndActive{ nullptr };
	FGameplayEffectSpecHandle RollingSpecHandle;
	FActiveGameplayEffectHandle RollingHandle;
	
};
