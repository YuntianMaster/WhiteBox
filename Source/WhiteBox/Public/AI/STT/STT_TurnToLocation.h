// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_TurnToLocation.generated.h"

class UAnimInstance;

/**
 * 
 */
UCLASS()
class WHITEBOX_API USTT_TurnToLocation : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Context")
	AActor* ActorRef;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
	FVector DestinationLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* L_90;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* L_180;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* R_90;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* R_180;

protected:
	/** @return true if montage is playing and task should stay Running */
	bool TurnToLocation();
	void CleanupWarpTarget();
	void ClearMontageEndDelegate();

	TWeakObjectPtr<UAnimInstance> CachedAnimInstance;
	TWeakObjectPtr<UAnimMontage> CachedTurnMontage;

public:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
