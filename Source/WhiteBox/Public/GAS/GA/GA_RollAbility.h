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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ROLLMontage")
	TArray<UAnimMontage*> Roll_F;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ROLLMontage")
	TArray<UAnimMontage*> Roll_R;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ROLLMontage")
	TArray<UAnimMontage*> Roll_B;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ROLLMontage")
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ROLLTag")
	float NormalDodgeMagnitude{ 0.5f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ROLLTag")
	float PerfectDodgeMagnitude{ 0.3f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ROLLTag")
	TSubclassOf<UGameplayEffect> GE_Dodge;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ROLLTag")
	TSubclassOf<UGameplayEffect> GE_PerfectDodge;

protected:
	ACharacter* CharacterRef{};
	int RollIndex{ 0 };
	class UAbilityTask_WaitDelay* DelayEndActive{ nullptr };
	FGameplayEffectSpecHandle RollingSpecHandle;
	FActiveGameplayEffectHandle RollingHandle;
	UFUNCTION()
	void ResetMeshDirection();

	enum class ERollDirection
	{
		NOINPUT,
		F,
		FR,
		R,
		BR,
		B,
		BL,
		L,
		FL

	};

	ERollDirection EROLLDir{ ERollDirection::NOINPUT };


	FActiveGameplayEffectHandle PerfectDodgeHandle;
	FActiveGameplayEffectHandle DodgeHandle;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
};
