// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_Action/BTT_Base.h"
#include "Enum/ECombatEnum.h"
#include "BT_ChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UBT_ChasePlayer : public UBTT_Base
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float AcceptRadiu;
	FScriptDelegate MoveCompleteDelegate;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CombatSpeed")
	bool bIsCutstomCombatSpeed{ false };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CombatSpeed", meta = (EditCondition = "bIsCutstomCombatSpeed"))
	float CustomCombatSpeed{ 2000.f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CombatSpeed", meta = (EditCondition = "!bIsCutstomCombatSpeed"))
	TEnumAsByte<ECombatSpeeds> CombatSpeeds{ ECombatSpeeds::WalkSpeed };

	UFUNCTION()
	void MoveComplete();
	bool bMoveComplete{false};
public:
	UBT_ChasePlayer();
	void ChaseToPlayer(UBehaviorTreeComponent& OwnerComp);
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

};
