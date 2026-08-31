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
	virtual void WarpMontageHandler();
	UFUNCTION(BlueprintCallable)
	virtual void NoWarpMontageHandle();

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
	//Tick����MontagePlayRate
	UFUNCTION()
	void UpdatePlayeRate(float DeltaTime);


	UFUNCTION(BlueprintImplementableEvent)
	void OnTraceSuccessBroadCast();
		
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



	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Montage")
	UAnimMontage* AttackMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Montage")
	bool bIsNeedCurveMontagePlayRate{ false };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bIsNeedCurveMontagePlayRate"), Category = "Montage")
	FName PlayRateCurve = "PlayRateCurve";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Warp")
	FName AttackWarpingName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Warp|EndLocation")
	FName EndWarpingName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Warp|EndLocation")
	FVector EndWarpingLocation;
	//True��׷�ٵ���Ŀ�꣬False׷��EQS��ѯ��ַ
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Warp")
	bool bIsActorTarget{ true };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PlayRate{1.f};
	class UMotionWarpingComponent* MWcomp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Warp")
	bool bTrackTarget{ true };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Warp|Offset")
	float WarpDistanceOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Warp|Offset")
	FVector WarpOffset;

	bool bEndWarpStarted = false;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DamageMagnitude;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "HitCritial")
	bool bIsCriticalHit{false};
	int32 MontageInstanceID;
};
