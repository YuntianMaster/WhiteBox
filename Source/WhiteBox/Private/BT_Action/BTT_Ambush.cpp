// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Action/BTT_Ambush.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

UBTT_Ambush::UBTT_Ambush(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_Ambush::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UE_LOG(LogTemp, Warning, TEXT("UBTT_Ambush::ExecuteTask (instance %s)"), *GetNameSafe(this));

	if (!CoverQuery)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTT_Ambush: CoverQuery is null."));
		return EBTNodeResult::Failed;
	}

	UObject* Querier = AI ? AI->GetPawn() : nullptr;
	if (!Querier)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTT_Ambush: Querier (AI Pawn) is null."));
		return EBTNodeResult::Failed;
	}

	CachedBTComp = &OwnerComp;

	FEnvQueryRequest Request(CoverQuery, Querier);

	// 很多引擎版本只有两参：RunMode + FQueryFinishedSignature（世界/Owner 取自 Request 构造时的 Querier）。
	FQueryFinishedSignature FinishDelegate =
		FQueryFinishedSignature::CreateUObject(this, &UBTT_Ambush::OnQueryFinished);

	Request.Execute(EEnvQueryRunMode::SingleResult, FinishDelegate);
	UE_LOG(LogTemp, Warning, TEXT("UBTT_Ambush: FEnvQueryRequest::Execute 已提交（异步：OnQueryFinished）"));

	return EBTNodeResult::InProgress;
}

void UBTT_Ambush::OnQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	UE_LOG(LogTemp, Warning, TEXT("UBTT_Ambush: OnQueryFinished (callback entered)"));

	UBehaviorTreeComponent* OwnerComp = CachedBTComp.Get();

	const auto FinishTask = [&](EBTNodeResult::Type ResultType)
	{
		CachedBTComp.Reset();
		if (OwnerComp)
		{
			FinishLatentTask(*OwnerComp, ResultType);
		}
	};

	if (!OwnerComp)
	{
		CachedBTComp.Reset();
		return;
	}

	FEnvQueryResult* R = Result.Get();
	if (!R || !R->IsSuccessful())
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTT_Ambush: EQS failed or invalid result."));
		FinishTask(EBTNodeResult::Failed);
		return;
	}

	TArray<FVector> Locations;
	R->GetAllAsLocations(Locations);
	if (!Locations.IsValidIndex(0))
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTT_Ambush: No passing locations."));
		FinishTask(EBTNodeResult::Failed);
		return;
	}

	const FVector SinglePoint = Locations[0];
	UE_LOG(LogTemp, Warning, TEXT("UBTT_Ambush: EQS point: %s"), *SinglePoint.ToString());
	BB->SetValueAsVector("PointofIntesting", SinglePoint);
	FinishTask(EBTNodeResult::Succeeded);	
}
