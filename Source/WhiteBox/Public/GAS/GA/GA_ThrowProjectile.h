// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA_PlayMontage.h"
#include "GAS/GA/GA_WarpMontageAttack.h"
#include "GA_ThrowProjectile.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UGA_ThrowProjectile : public UGA_WarpMontageAttack
{
	GENERATED_BODY()
protected:


protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UFUNCTION()
	void ShowWeaponHandle(FGameplayEventData Payload);
	UFUNCTION()
	void RemoveWeaponHandle(FGameplayEventData Payload);
	UFUNCTION()
	void ThrowPrimeWeaponHandle(FGameplayEventData Payload);
	UFUNCTION()
	void ThrowSecondWeaponHandle(FGameplayEventData Payload);


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BezierSettings")
	bool bPrimeUp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BezierSettings")
	bool bPrimeRight;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BezierSettings")
	bool bSecondUp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BezierSettings")
	bool bSecondRight;
};
