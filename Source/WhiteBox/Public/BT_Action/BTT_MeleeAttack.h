// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"
#include "BTT_MeleeAttack.generated.h"


/**
 * 
 */
UCLASS()
class WHITEBOX_API UBTT_MeleeAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTT_MeleeAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float AcceptRadius{ 150.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeleeAttackRange{ 200.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeleeRange{ 1250.f};
	FScriptDelegate MoveDelegate;
	UFUNCTION()
	void FinishAttack();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMoveComplete{ false };

	void OnMoveCompleted();

	bool bIsAttackComplete{ true };
};
