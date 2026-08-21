 // Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Action/BTT_FocusTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

EBTNodeResult::Type UBTT_FocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* const AI = OwnerComp.GetAIOwner();
	UBlackboardComponent* const BB = OwnerComp.GetBlackboardComponent();
	if (!AI || !BB)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BTT_FocusTarget] AI or Blackboard null (AI=%p BB=%p)"), AI, BB);
		return EBTNodeResult::Failed;
	}

	static const FName TargetKey(TEXT("TargetActor"));
	UObject* const TargetObject = BB->GetValueAsObject(TargetKey);
	AActor* const TargetActor = Cast<AActor>(TargetObject);
	if (!TargetActor)
	{
		if (TargetObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("[BTT_FocusTarget] Blackboard '%s' is not an Actor (Class=%s), clearing focus"),
				*TargetKey.ToString(), *TargetObject->GetClass()->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[BTT_FocusTarget] Blackboard '%s' is null, clearing focus"), *TargetKey.ToString());
		}
		AI->ClearFocus(EAIFocusPriority::Gameplay);
		return EBTNodeResult::Failed;
	}

	AI->SetFocus(TargetActor, EAIFocusPriority::Gameplay);
	UE_LOG(LogTemp, Log, TEXT("[BTT_FocusTarget] %s focused on %s"), *AI->GetName(), *TargetActor->GetName());
	return EBTNodeResult::Succeeded;
}
