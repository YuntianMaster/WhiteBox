// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Action/BTT_Base.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AttrubuteSet/CombatAttributeSet.h"

EBTNodeResult::Type UBTT_Base::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	Super::ExecuteTask(OwnerComp, NodeMemory);
	BB = OwnerComp.GetBlackboardComponent();
	ASC = OwnerComp.GetAIOwner()->GetPawn()->FindComponentByClass<UAbilitySystemComponent>();
	AI = OwnerComp.GetAIOwner();
	PerceptionComp = AI->GetComponentByClass<UAIPerceptionComponent>();
	CombatAttributes = Cast<UCombatAttributeSet>(ASC->GetAttributeSet(UCombatAttributeSet::StaticClass()));
	ActorRef = AI->GetPawn();
	TargetActor = Cast<AActor>(BB->GetValueAsObject("TargetActor"));	
	UE_LOG(LogTemp, Warning, TEXT("ExecuteTask UBTT_Base (task class: %s)"), *GetClass()->GetName());
	return EBTNodeResult::Succeeded;
}
