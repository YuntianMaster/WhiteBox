// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Action/BT_ChasePlayer.h"
#include "AIController.h"
#include "Player/Estats.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/StatsComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

 UBT_ChasePlayer::UBT_ChasePlayer() {

	 bNotifyTick = true;
	 MoveCompleteDelegate.BindUFunction(this, "MoveComplete");
}

 void UBT_ChasePlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
 {
	 if (OwnerComp.GetAIOwner()->GetMoveStatus() == EPathFollowingStatus::Idle)
		 bMoveComplete = true;
	if (!bMoveComplete) return;
	ControllerRef->ReceiveMoveCompleted.RemoveAll(this);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

 }

 void UBT_ChasePlayer::ChaseToPlayer(UBehaviorTreeComponent& OwnerComp)
 {

	 bMoveComplete = false;
	 ACharacter* CharRef = OwnerComp.GetAIOwner()->GetPawn<ACharacter>();
	 UStatsComponent* StatsComp = CharRef->GetComponentByClass<UStatsComponent>();
	 CharRef->GetCharacterMovement()->MaxWalkSpeed = StatsComp->Stats[EStats::BattleWalkSpeed];
	 FAIMoveRequest MoveRequest{ GetWorld()->GetFirstPlayerController()->GetPawn() };
	 MoveRequest.SetAcceptanceRadius(AcceptRadiu);
	 MoveRequest.SetUsePathfinding(true);
	 ControllerRef->MoveTo(MoveRequest);
	 ControllerRef->ReceiveMoveCompleted.AddUnique(MoveCompleteDelegate);
 }

 void UBT_ChasePlayer::MoveComplete()
 {
	 bMoveComplete = true;
 }

 EBTNodeResult::Type UBT_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
 {

	ControllerRef = OwnerComp.GetAIOwner();
	ChaseToPlayer(OwnerComp);
	return EBTNodeResult::InProgress;
 }
