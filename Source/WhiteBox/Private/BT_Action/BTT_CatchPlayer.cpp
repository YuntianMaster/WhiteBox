// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Action/BTT_CatchPlayer.h"
#include "Interface/Fighter.h"
#include "Enemy/EnemyCharacter.h"
#include "GameFramework/Character.h"
#include "AIController.h"

UBTT_CatchPlayer::UBTT_CatchPlayer()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_CatchPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	IFighter* FightRef = Cast<IFighter>(OwnerComp.GetAIOwner()->GetCharacter());
	if (!FightRef) EBTNodeResult::Failed;
	FightRef->Catch();
	return EBTNodeResult::InProgress;
}

void UBTT_CatchPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetCharacter())->bIsCatchComplete)
	{
		Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetCharacter())->bIsCatchComplete = false;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
