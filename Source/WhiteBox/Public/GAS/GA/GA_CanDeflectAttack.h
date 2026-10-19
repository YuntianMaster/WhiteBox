// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/GA_WarpMontageAttack.h"
#include "GA_CanDeflectAttack.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct WHITEBOX_API FFailAttackAndDeflectAnimStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FailAttackAnimPlayRate = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* FailAttackAnim = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WarpOffset = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName WarpName{"Deflect"};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* DefectAnim = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsFaceToTarget;
};

UCLASS()
class WHITEBOX_API UGA_CanDeflectAttack : public UGA_WarpMontageAttack
{
	GENERATED_BODY()
public:
	
	virtual void OnTraceHitHandle(FGameplayEventData Payload) override;
	

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SuperParry")
	FGameplayTag DeflectTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SuperParry")
	bool bIsCurveDeflectPlayRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SuperParry", meta = (EditCondition = "bIsCurveDeflectPlayRate"))
	FName DeflectCurveName{"PlayRate"};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SuperParry")
	TArray<FFailAttackAndDeflectAnimStruct> DeflectAnims;


	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void DeflectHandler(FGameplayEventData Payload);

	void GetDeflectAnim();
	UFUNCTION()
	void UpdateDeflectAnimPlayeRate(float DeltaTime);
	UAnimMontage* FailAttackAnimMontage;
	UAnimMontage* DefectAnimMontage;
	float FailAttackWarpOffset;
	float FailAttackAnimMontagePlayRate;
	FName DefectWarpingName;
	bool bIsDeflectFaceToTarget;
};
