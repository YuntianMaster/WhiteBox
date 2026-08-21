// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Action/BTT_Hide_Ambush.h"
#include "GameFramework/Character.h"
#include "Enemy/EnemyCharacter.h"
#include "Enemy/EEnemyStats.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "AIController.h"



UBTT_Hide_Ambush::UBTT_Hide_Ambush()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Hide_Ambush::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyCharacter* ControlledActor = Cast<AEnemyCharacter>(AI->GetPawn());
	if (ControlledActor)
	{
		ControlledActor->EnemyStates = EEnemyStats::Hide;
	}
	return EBTNodeResult::InProgress;
}

void UBTT_Hide_Ambush::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if(HasSeenTarget())
	{

		if (!AmbushMontage) {

			UE_LOG(LogTemp, Warning, TEXT("UBTT_Hide_Ambush: AmbushMontage is null."));
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		ACharacter* CherRef = Cast<ACharacter>(AI->GetPawn());
		if(!CherRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("UBTT_Hide_Ambush: CherRef is null."));
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}
		CherRef->PlayAnimMontage(AmbushMontage);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
}


bool UBTT_Hide_Ambush::HasSeenTarget()
{
	bool isSeen = PerceptionComp->HasActiveStimulus(*TargetActor, UAISense::GetSenseID<UAISense_Sight>());
	return isSeen;
}
