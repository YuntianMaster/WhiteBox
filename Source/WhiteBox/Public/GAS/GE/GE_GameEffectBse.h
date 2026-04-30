// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_GameEffectBse.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UGE_GameEffectBse : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool bHasDebuff = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bHasDebuff"), Category = "UI")
	UTexture2D* DebuffIcon;
	
};
