// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/GA_GameAblilityBase.h"
#include "GA_ParryReaction.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UGA_ParryReaction : public UGA_GameAblilityBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ParryAnim")
	TArray<UAnimMontage*> UL_M_ParryAnim;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ParryAnim")
	TArray<UAnimMontage*> UR_M_ParryAnim;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ParryAnim")
	TArray<UAnimMontage*> DL_M_ParryAnim;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ParryAnim")
	TArray<UAnimMontage*> DR_M_ParryAnim;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CritcalParryAnim")
	TArray<UAnimMontage*> UL_M_CriticalParryAnim;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CritcalParryAnim")
	TArray<UAnimMontage*> UR_M_CriticalParryAnim;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CritcalParryAnim")
	TArray<UAnimMontage*> DL_M_CriticalParryAnim;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CritcalParryAnim")
	TArray<UAnimMontage*> DR_M_CriticalParryAnim;







	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parry")
	TSubclassOf<UGameplayEffect> BeParriedGE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parry")
	float ParryPoiseMagnitude{ 10.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parry")
	float ParryLastMagnitude{ 0.3f };


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ParryAngle = 45.f;
	UFUNCTION()
	void EndAbilityHandler();
	UFUNCTION()
	void ParrySuccessHandle(FGameplayEventData Playload);
	UFUNCTION()
	void CriticalParryHandle(FGameplayEventData Playload);


public:
	UFUNCTION(BlueprintCallable)
	void ParryHandle(FGameplayEventData Playload);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
