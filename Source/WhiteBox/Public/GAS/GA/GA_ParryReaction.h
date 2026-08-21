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
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<UAnimMontage*> UL_M_ParryAnim;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UAnimMontage*> UR_M_ParryAnim;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UAnimMontage*> DL_M_ParryAnim;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UAnimMontage*> DR_M_ParryAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parry")
	TSubclassOf<UGameplayEffect> BeParriedGE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ParryAngle = 45.f;
	void EndAbilityHandler();
	void ParrySuccessHandle(FGameplayEventData Playload);



public:
	UFUNCTION(BlueprintCallable)
	void ParryHandle(FGameplayEventData Playload);
};
