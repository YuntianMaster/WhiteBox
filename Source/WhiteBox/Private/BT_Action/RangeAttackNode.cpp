// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Action/RangeAttackNode.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/EBossStats.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Interface/Fighter.h"


EBTNodeResult::Type URangeAttackNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* CharacterRef{ 
	OwnerComp.GetAIOwner()->GetPawn<ACharacter>()
	};
	if (!IsValid(CharacterRef))
		return EBTNodeResult::Failed;

	float distance{
		OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance"))
	};

	AAIController* AIRef{ OwnerComp.GetAIOwner() };
	IFighter* FighterRef{ Cast<IFighter>(AIRef->GetCharacter()) };
	
		if (distance < FighterRef->GetMeleeRange())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BossStats"), EBossStats::Melee);
		AbortTask(OwnerComp,NodeMemory);
		return EBTNodeResult::Aborted;
	}
	CharacterRef->PlayAnimMontage(AnimMontage);

	double RandomValue{ UKismetMathLibrary::RandomFloat() };

	if (RandomValue > Threshold) {

		Threshold = 0.9;
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BossStats"), EBossStats::Charge);
	}

	else Threshold -= 0.1;

	return EBTNodeResult::Type::Succeeded;
}


