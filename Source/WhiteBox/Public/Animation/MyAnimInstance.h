 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float Currentvelocity{ 0.0f };
	UFUNCTION(BlueprintCallable)
	void UpdateVelocity();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentDirection{ 0.0f };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsIncombat{ false };
};
 