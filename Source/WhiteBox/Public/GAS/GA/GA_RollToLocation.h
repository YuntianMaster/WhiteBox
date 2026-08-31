// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/GA_WarpMontageAttack.h"
#include "Structure/FMovementAnimStruct.h"
#include "GA_RollToLocation.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UGA_RollToLocation : public UGA_WarpMontageAttack
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RollAnimation")
	TMap<TEnumAsByte<EPlayerVelocityDirection>, UAnimMontage*> RollMontages;

	EPlayerVelocityDirection GetRollDirection();
	
	virtual void OnEQSFinished(TSharedPtr<FEnvQueryResult> Result) override;
};
