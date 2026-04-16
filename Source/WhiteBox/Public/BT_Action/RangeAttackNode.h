// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RangeAttackNode.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API URangeAttackNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
		
		UPROPERTY(EditAnywhere)
		UAnimMontage* AnimMontage;
		UPROPERTY(EditAnywhere)
		double Threshold{ 0.9 };

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MeleeRange{ 1250.f };

		
};
