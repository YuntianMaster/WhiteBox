// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	UPROPERTY(BlueprintReadWrite)
	float CurrentVerticleSpeed{ 0.0f };
	UFUNCTION(BlueprintCallable)
	void UpdateCurrentVerticleSpeed();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bIsCharging{ false };
	
};
