// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/GA_GameAblilityBase.h"
#include "GA_Bow_ShootAbility.generated.h"

class AGameplayAbilityTargetActor_SingleLineTrace;
/**
 * 
 */
UCLASS()
class WHITEBOX_API UGA_Bow_ShootAbility : public UGA_GameAblilityBase
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void BowCharing();
	UPROPERTY(BlueprintReadWrite)
	float CharingTime{0.f};
	FTimerHandle FCharingHandle;
	UFUNCTION()
	void ReAimingHandle();

	/** 与 UE5.5 UAbilityTask_SpawnActor::DidNotSpawn 一致：void(AActor*)，失败时指针多为 nullptr */
	UFUNCTION()
	void OnArrowSpawnFailed(AActor* SpawnedActor);

	/** 箭矢生成成功后再播射击蒙太奇，避免与 SpawnActor 任务并发时被 EndAbility 取消 */
	void StartShootMontageThenReAim();

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* BowDrawMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* BowShootMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxCharingTime{ 2.5f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AGameplayAbilityTargetActor_SingleLineTrace> TargetClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> ShootArrowActor;
	UFUNCTION()
	void CharingTimeHandle();

	UFUNCTION()
	void OnBowTargetDataValid(const FGameplayAbilityTargetDataHandle& Data);
	UFUNCTION()
	void FireArrow(AActor* SpawnActors);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
};
