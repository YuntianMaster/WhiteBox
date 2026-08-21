// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_FlyToLocationOrActor.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API USTT_FlyToLocationOrActor : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Context")
	class AEnemyAIController* EnemyAIController;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Context")
	class AEnemyCharacter* EnemyCharacter;

protected:

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Radius{100.f};
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MovementSpeed{ 1000.f };
	AActor* TargetActor;
};
