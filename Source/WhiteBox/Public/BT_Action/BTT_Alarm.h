// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Alarm.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UBTT_Alarm : public UBTTaskNode
{
	GENERATED_BODY()
	
public :
	void StartEnemyAlarm(UBehaviorTreeComponent& OwnerComp);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	USoundBase* AlarmSound;
};
