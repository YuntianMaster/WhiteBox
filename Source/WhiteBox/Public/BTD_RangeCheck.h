// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_RangeCheck.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UBTD_RangeCheck : public UBTDecorator
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RangeCheck")
	FName DistanceKey;
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
