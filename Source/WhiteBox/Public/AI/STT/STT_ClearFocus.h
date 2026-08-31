// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_ClearFocus.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API USTT_ClearFocus : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Context")
	class AEnemyCharacter* ActorRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Context")
	class AEnemyAIController* EnemyController;
protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	void CancelFocusTarget();
};
