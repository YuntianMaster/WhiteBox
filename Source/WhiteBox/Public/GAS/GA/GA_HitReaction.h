// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/GA_GameAblilityBase.h"
#include "GA_HitReaction.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UGA_HitReaction : public UGA_GameAblilityBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UAnimMontage* M_HitReaction_F;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* M_HitReaction_FL;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* M_HitReaction_FR;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* M_HitReaction_B;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* M_HitReaction_L;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* M_HitReaction_R;
public:
	UFUNCTION(BlueprintCallable)
	void GetHit(FVector HitLocation);
	UFUNCTION()
	void OnHitReactionMontageCompleted();
	
	
};
