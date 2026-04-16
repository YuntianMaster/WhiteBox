// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_ChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UBT_ChasePlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBT_ChasePlayer();
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	class AAIController* ControllerRef;
	void ChaseToPlayer(UBehaviorTreeComponent& OwnerComp);
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float AcceptRadiu;
	FScriptDelegate MoveCompleteDelegate;
	UFUNCTION()
	void MoveComplete();
	bool bMoveComplete{false};

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

};
