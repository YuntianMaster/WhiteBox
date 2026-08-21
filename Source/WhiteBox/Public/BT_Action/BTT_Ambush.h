// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BT_Action/BTT_Base.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "BTT_Ambush.generated.h"

class UEnvQuery;

/**
 *
 */
UCLASS()
class WHITEBOX_API UBTT_Ambush : public UBTT_Base
{
	GENERATED_BODY()

public:
	UBTT_Ambush(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UEnvQuery> CoverQuery = nullptr;

	void OnQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	UPROPERTY(Transient)
	TWeakObjectPtr<UBehaviorTreeComponent> CachedBTComp;
};
