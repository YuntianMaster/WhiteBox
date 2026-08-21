// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Action/BTT_MeleeAttack.h"
#include "AIController.h"
#include "Enemy/BossCharater.h"
#include "Interface/Fighter.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "AttrubuteSet/CombatAttributeSet.h"
#include "Navigation/PathFollowingComponent.h"





EBTNodeResult::Type UBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{


	AAIController* const AI = OwnerComp.GetAIOwner();
	UBlackboardComponent* const BB = OwnerComp.GetBlackboardComponent();
	if (!AI || !BB)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BTT_FocusTarget] AI or Blackboard null (AI=%p BB=%p)"), AI, BB);
		return EBTNodeResult::Failed;
	}

	UAbilitySystemComponent* ASC = AI->GetPawn()->GetComponentByClass<UAbilitySystemComponent>();
	static const FName TargetKey(TEXT("TargetActor"));
	UObject* const TargetObject = BB->GetValueAsObject(TargetKey);
	AActor* const TargetActor = Cast<AActor>(TargetObject);
	
	if(!ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ASC!"));
		return EBTNodeResult::Failed;
	}

	if(!ASC->GetAttributeSet(UCombatAttributeSet::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("No AttributeSet!"));
		return EBTNodeResult::Failed;
	}
	float MeleeAttackRange = Cast<UCombatAttributeSet>(ASC->GetAttributeSet(UCombatAttributeSet::StaticClass()))->GetMeleeAttackRange();


	if(!TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Target!"));
		return EBTNodeResult::Failed;
	}


	ASC->TryActivateAbilitiesByTag(FGameplayTagContainer{ FGameplayTag::RequestGameplayTag(TEXT("GamePlayAbility.Combat.Attack.Melee")) }, false);
	return EBTNodeResult::Succeeded;
}




