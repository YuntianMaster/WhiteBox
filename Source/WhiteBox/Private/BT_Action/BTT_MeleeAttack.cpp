// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Action/BTT_MeleeAttack.h"
#include "AIController.h"
#include "Enemy/BossCharater.h"
#include "Interface/Fighter.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"




UBTT_MeleeAttack::UBTT_MeleeAttack() {

	bNotifyTick = true;
	MoveDelegate.BindUFunction(this, "OnMoveCompleted");
}

EBTNodeResult::Type UBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	//UE_LOG(LogTemp, Warning, TEXT("START"));
	bIsMoveComplete = false;
	float distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance"));
	AAIController* AIRef{ OwnerComp.GetAIOwner() };
	if (distance > MeleeAttackRange) 
	{
		
		FAIMoveRequest MoveRequest{ OwnerComp.GetWorld()->GetFirstPlayerController()->GetPawn() };
		MoveRequest.SetUsePathfinding(true);
		MoveRequest.SetAcceptanceRadius(AcceptRadius);
		AIRef->MoveTo(MoveRequest);
		AIRef->ReceiveMoveCompleted.AddUnique(MoveDelegate);
	}


	else {

		bIsAttackComplete = false;
		IFighter* FigtherRef{
			Cast<IFighter>(AIRef->GetCharacter())
		};
		FigtherRef->Attack();

		FTimerHandle AttackTimeHandle;

		AIRef->GetCharacter()->GetWorldTimerManager().SetTimer(
			AttackTimeHandle,
			this,
			&UBTT_MeleeAttack::FinishAttack,
			FigtherRef->GetAttackDuration());
	}
	return EBTNodeResult::Type::InProgress;
}




void UBTT_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	AAIController* AIRef{ OwnerComp.GetAIOwner() };
	IFighter* FigtherRef{
			Cast<IFighter>(AIRef->GetCharacter())
	};
	float distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance"));
	if (distance > FigtherRef->GetMeleeRange()) {

		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BossStats"), EBossStats::Range);
		AbortTask(OwnerComp, NodeMemory);
		FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
		AIRef->StopMovement();
		AIRef->ReceiveMoveCompleted.Remove(MoveDelegate);
	}
	
	
	
	if (!bIsMoveComplete)
	{
		if (AIRef->GetMoveStatus() == EPathFollowingStatus::Idle)
		{
			// AI 已经停止但未触发 MoveCompleted
			bIsMoveComplete = true;
		}
		else
		{
			return;
		}
	}

	if (!bIsAttackComplete) return;
	
	
	OwnerComp.GetAIOwner()->ReceiveMoveCompleted.Remove(MoveDelegate);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

void UBTT_MeleeAttack::FinishAttack()
{
	bIsAttackComplete = true;
}


void UBTT_MeleeAttack::OnMoveCompleted()
{
	bIsMoveComplete = true;
}

