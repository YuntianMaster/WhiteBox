// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Action/BT_Enemy_Patrol.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Player/Estats.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/StatsComponent.h"

UBT_Enemy_Patrol::UBT_Enemy_Patrol()
{
	bNotifyTick = true;
	NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	MoveDelegate.BindUFunction(this,"ArrivedHandle");
}

void UBT_Enemy_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	if (isArrived)
	{
		AAIController* controllerRef = OwnerComp.GetAIOwner();
		controllerRef->ReceiveMoveCompleted.RemoveAll(this);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBT_Enemy_Patrol::RandomPatrol(UBehaviorTreeComponent& OwnerComp)
{
	AAIController* controllerRef = OwnerComp.GetAIOwner();
	isArrived = false;
	APawn* pawn = controllerRef->GetPawn();
	FVector playerloc = pawn->GetActorLocation();
	if (NavSys)
	{
		FNavLocation NewLocation;
		NavSys->GetRandomReachablePointInRadius(playerloc, PatrolRadius, NewLocation,nullptr);
		playerloc = NewLocation.Location;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Move %s"), *playerloc.ToString())
	FAIMoveRequest MoveRequest{ playerloc };
	MoveRequest.SetUsePathfinding(true);
	MoveRequest.SetAcceptanceRadius(AcceptRadiu);
	controllerRef->MoveTo(MoveRequest);
	controllerRef->ReceiveMoveCompleted.AddUnique(MoveDelegate);
}

void UBT_Enemy_Patrol::LinePartrol(UBehaviorTreeComponent& OwnerComp)
{
	if (PatrolPoint.Num() == 0) return;
	AAIController* controllerRef =  OwnerComp.GetAIOwner();
	isArrived = false;
	FAIMoveRequest MoveRequest{ PatrolPoint[PointIndex] };
	MoveRequest.SetUsePathfinding(true);
	MoveRequest.SetAcceptanceRadius(AcceptRadiu);
	controllerRef->MoveTo(MoveRequest);
	controllerRef->ReceiveMoveCompleted.AddUnique(MoveDelegate);


}

void UBT_Enemy_Patrol::ArrivedHandle()
{
	isArrived = true;
	PointIndex++;
	PointIndex = UKismetMathLibrary::Wrap(PointIndex, -1, PatrolPoint.Num() - 1);
}

EBTNodeResult::Type UBT_Enemy_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ChaRef = OwnerComp.GetAIOwner()->GetPawn<ACharacter>();
	StatsComp = ChaRef->GetComponentByClass<UStatsComponent>();
	if (StatsComp && ChaRef)
	{
		ChaRef->GetCharacterMovement()->MaxWalkSpeed = StatsComp->Stats[EStats::PatrolWalkSpeed];
		ChaRef->GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	}
	if (bIsRandomPatrol)
		RandomPatrol(OwnerComp);
	else
		LinePartrol(OwnerComp);
	return EBTNodeResult::Type::InProgress;

}


