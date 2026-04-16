// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "GameplayEffectTypes.h"
#include "GAS/GA/GA_GameAblilityBase.h"
#include "GA_BowAimingAbility.generated.h"

/**
 * 
 */

class AGameplayAbilityTargetActor_SingleLineTrace;

UCLASS()
class WHITEBOX_API UGA_BowAimingAbility : public UGA_GameAblilityBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void AimingStart();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UAnimMontage* AimingMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AGameplayAbilityTargetActor_SingleLineTrace> TargetClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> Arrow;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	float NormalSprintSpeed;
	float NormalWalkSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AimWalkSpeed{ 200.f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AimSprintSpeed{ 400.f };
	UFUNCTION()
	void CancelAimingHandle(FGameplayEventData Payload);
	UFUNCTION()
	void ShootingHandle(FGameplayEventData Payload);


	UFUNCTION()
	void GetArrow(FGameplayEventData Payload);
	/** WaitTargetData::ValidData — 参数必须是 TargetDataHandle，不能与 ShootingHandle 混用 */


	AActor* CreatedArrow;
};
