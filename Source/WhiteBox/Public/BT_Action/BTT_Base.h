// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Base.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UBTT_Base : public UBTTaskNode
{
	GENERATED_BODY()

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UBlackboardComponent* BB;
	class AAIController* ControllerRef;
	AActor* ActorRef;
	class UAbilitySystemComponent* ASC;
	const class UCombatAttributeSet* CombatAttributes;
	AActor* TargetActor;
	AAIController* AI;
	class UAIPerceptionComponent* PerceptionComp;
};
