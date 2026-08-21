// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BT_Action/BTT_Base.h"
#include "BTT_Hide_Ambush.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UBTT_Hide_Ambush : public UBTT_Base
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* HideMontage;
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* AmbushMontage;
	bool HasSeenTarget();

public:
	UBTT_Hide_Ambush();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	
};
