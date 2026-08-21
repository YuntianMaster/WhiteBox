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
#include "Enum/ECombatEnum.h"
#include "Enemy/EnemyAIController.h"
#include "AttrubuteSet/CombatAttributeSet.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

 UBT_ChasePlayer::UBT_ChasePlayer() {

	 bNotifyTick = true;
	 MoveCompleteDelegate.BindUFunction(this, "MoveComplete");
}

 void UBT_ChasePlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
 {
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return;
	}
	if (AIController->GetMoveStatus() == EPathFollowingStatus::Idle)
	{
		bMoveComplete = true;
	}
	if (!bMoveComplete)
	{
		return;
	}
	AIController->ReceiveMoveCompleted.RemoveAll(this);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
 }

 void UBT_ChasePlayer::ChaseToPlayer(UBehaviorTreeComponent& OwnerComp)
 {

	 bMoveComplete = false;

	 if(!TargetActor)
	 {
		 UE_LOG(LogTemp, Warning, TEXT("No Target!"));
		 return FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		
	 } 
	 ACharacter* CharRef = OwnerComp.GetAIOwner()->GetPawn<ACharacter>();
	 if(!CharRef)
	 {
		 UE_LOG(LogTemp, Warning, TEXT("No CharRef!"));
		 return FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	 }

	 if(!CombatAttributes)
	 {
		 UE_LOG(LogTemp, Warning, TEXT("No CombatAttributes!"));
		 return FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	 }

	 AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(AI);
	 
	 if (!EnemyAIController) {
		 UE_LOG(LogTemp, Warning, TEXT("No EnemyAIController in ChaseToPlayer!"));
		 return FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	 }

	 if (bIsCutstomCombatSpeed) {

		// EnemyAIController->SetEnemyCombatSpeedsByFloat(CustomCombatSpeed);

	 }
	 else
	 {
		// EnemyAIController->SetEnemyCombatSpeedsByEnum(CombatSpeeds);
	 }

	 FAIMoveRequest MoveRequest{ TargetActor };
	 MoveRequest.SetAcceptanceRadius(AcceptRadiu);
	 MoveRequest.SetUsePathfinding(true);
	 AI->MoveTo(MoveRequest);
	 AI->ReceiveMoveCompleted.AddUnique(MoveCompleteDelegate);
 }

 void UBT_ChasePlayer::MoveComplete()
 {
	 bMoveComplete = true;
 }

 EBTNodeResult::Type UBT_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
 {
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ChaseToPlayer(OwnerComp);
	return EBTNodeResult::InProgress;
 }
