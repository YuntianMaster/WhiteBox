// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GAS/GA/GA_GameAblilityBase.h"
#include "GA_ComboAbility.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UGA_ComboAbility : public UGA_GameAblilityBase
{
	GENERATED_BODY()

protected:
	int CombotCounter{ 0 };
	bool bCanComboInput{ true };

	

public:
	UGA_ComboAbility();
	UFUNCTION()
	void OnCancelHandle();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WarpMotion")
	float MotionWarpDistance{ 300.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WarpMotion", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float TargetOffset{ 0.8f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> ComboMontages;

	UFUNCTION(BlueprintCallable)
	virtual void GA_ComboAttackHandle();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackTimingPercent{ 0.7f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackBufferTime{0.3f};

	FTimerHandle GA_FCombotInputHandle;
	FTimerHandle GA_FCombotInputPassHandle;

	UFUNCTION()
	void GA_FCombotInput();
	UFUNCTION()
	void GA_FCombotInputPass();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GE_Attacking;
	FGameplayEffectSpecHandle AttackingSpecHandle;
	FActiveGameplayEffectHandle AttackingHandle;
protected:
	UAnimMontage* CurrentMontage;
};
