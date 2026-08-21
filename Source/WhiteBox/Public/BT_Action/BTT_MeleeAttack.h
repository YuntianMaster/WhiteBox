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
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float AcceptRadius{ 150.f };

};
