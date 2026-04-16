// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/GA_GameAblilityBase.h"
#include "GA_GameAblilityBase_TEST.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UGA_GameAblilityBase_TEST : public UGA_GameAblilityBase
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void TESTAbility();
	void TESTGameplayTask();
	UFUNCTION()
	void SpawnSucess(AActor* SpawnActors);
	UFUNCTION()
	void SpawnFail(AActor* SpawnActors);

	/** 用于 UAbilityTask_SpawnActor 测试；SpawnActor 需要非空的 FGameplayAbilityTargetDataHandle（UE5.5 用 FGameplayAbilityTargetData_LocationInfo，见 TEST cpp）。 */
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TSubclassOf<AActor> TestActor;
};
