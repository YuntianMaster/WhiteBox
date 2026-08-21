// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "AbilitySystemComponent.h"
#include "STT_ActiveGA.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API USTT_ActiveGA : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Context")
	TObjectPtr<AActor> Actor;

	UPROPERTY(Transient)
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<UGameplayAbility> AbilityClass;

	bool bActivated{ false };
	FGameplayAbilitySpecHandle ActiveHandle;

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

	void HandleAbilityEnded(const FAbilityEndedData& Data);
	void UnbindAbilityEnded();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DelayAfterGAFinish")
	bool bIsDelayAfterGAFinish{ false };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bIsDelayAfterGAFinish"), Category = "DelayAfterGAFinish")
	float DelayTime{ 1.f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bIsDelayAfterGAFinish"), Category = "DelayAfterGAFinish")
	float DelayTimeOffset{ 0.5f };
	FTimerHandle DelayFinishHandle;
	bool bPendingFinishSucceeded{ false };

	UFUNCTION()
	void DelayFinishHandler();

};
