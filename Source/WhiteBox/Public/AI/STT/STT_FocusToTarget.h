// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_FocusToTarget.generated.h"

/**
 * 设置 Focus；可选立刻 Succeeded，或等到朝向目标后再 Succeeded。
 */
UCLASS()
class WHITEBOX_API USTT_FocusToTarget : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Context")
	class AEnemyCharacter* ActorRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Context")
	class AEnemyAIController* EnemyController;

	/** true：等到面向目标后再 Succeeded；false：SetFocus 后立刻 Succeeded */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus")
	bool bWaitUntilFacing = false;

	/** 与目标朝向的 yaw 误差小于此值（度）时视为转向完成（仅 bWaitUntilFacing 时有效） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus", meta = (EditCondition = "bWaitUntilFacing", ClampMin = "0.0"))
	float AcceptanceAngle = 10.f;

protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
	void FocusOnTarget();
	bool IsFacingTarget() const;
};
