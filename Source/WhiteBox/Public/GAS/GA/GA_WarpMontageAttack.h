// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/GA_GameAblilityBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GA_WarpMontageAttack.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UGA_WarpMontageAttack : public UGA_GameAblilityBase
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintCallable)
	void WarpMontageHandler();
	UFUNCTION(BlueprintCallable)
	void NoWarpMontageHandle();

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void OnAttackCompleted();

	UFUNCTION(BlueprintImplementableEvent)
	void OnAttackCompletedBP();

	void WarpTarget_TRACKING();
	void WarpTarget_NOTRACKING();

	bool GroundCheckHandle();
	void StartMovementModeChangeHandle();
	void EndMovementModeChangeHandle();
	UFUNCTION()
	void WaitMovementModeHandle(ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode);
	UFUNCTION()
	void WaitMovementModeTimeHandle();
	UFUNCTION()
	void OnMotionWarpUpdate(class UMotionWarpingComponent* UWC);
	UFUNCTION()
	void OnTraceHitHandle(FGameplayEventData Payload);
		
protected:

	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//bool isAutoEnAblitiy{ true };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GroundCheck")
	bool bIsNeedGroundCheck{false};
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bisNeedGroundCheck"), Category = "GroundCheck")
	bool bIsNeedGround{true};
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ChangeMovementMode")
	bool bIsNeedchangeMovementMode{ false };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bIsNeedchangeMovementMode"), Category = "ChangeMovementMode")
	TEnumAsByte<EMovementMode> StartMovementMode{ EMovementMode::MOVE_Walking };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bIsNeedchangeMovementMode"), Category = "ChangeMovementMode")
	TEnumAsByte<EMovementMode> EndMovementMode{ EMovementMode::MOVE_Falling };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WaitEndMovementMode")
	bool bIsNeedWaitEndMovementMode{ false };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bIsNeedWaitEndMovementMode"), Category = "WaitEndMovementMode")
	TEnumAsByte<EMovementMode> WaitMovementMode{ EMovementMode::MOVE_Walking };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bIsNeedWaitEndMovementMode"), Category = "WaitEndMovementMode")
	float WaitTime{ 1.f};
	FTimerHandle WaitTimeHandle;



	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UAnimMontage* AttackMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName AttackWarpingName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EndLocation")
	FName EndWarpingName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EndLocation")
	FVector EndWarpingLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PlayRate;
	class UMotionWarpingComponent* MWcomp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bTrackTarget{ true };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WarpDistanceOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector WarpOffset;

	bool bEndWarpStarted = false;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DamageMagnitude;
	int32 MontageInstanceID;
};
