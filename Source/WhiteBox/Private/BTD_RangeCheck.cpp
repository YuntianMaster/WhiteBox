// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_RangeCheck.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTD_RangeCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	
	float CheckDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(DistanceKey);
	float Distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("Distance");


	if(Distance < CheckDistance)
		return true;
	return false;
}
