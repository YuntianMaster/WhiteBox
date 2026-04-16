// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_Enemy_Patrol.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UBT_Enemy_Patrol : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBT_Enemy_Patrol();

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	bool bIsRandomPatrol{ false };
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FVector> PatrolPoint;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float AcceptRadiu{ 20.f };
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float PatrolRadius{ 500.f };
	void RandomPatrol(UBehaviorTreeComponent& OwnerComp);
	void LinePartrol(UBehaviorTreeComponent& OwnerComp);
	UPROPERTY(VisibleAnywhere)
	int PointIndex = 0;
	

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	bool isArrived{ false };
	UFUNCTION()
	void ArrivedHandle();
	FScriptDelegate MoveDelegate;
	class UStatsComponent* StatsComp;
	ACharacter* ChaRef;
	class UNavigationSystemV1* NavSys;
		
};
