// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Action/BTS_PlayerDistance.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy/EBossStats.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTS_PlayerDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FVector PlayerLocation{ GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() };
	FVector EnemyLocation{ OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation() };
	Distance = static_cast<float>(UKismetMathLibrary::Vector_Distance(EnemyLocation, PlayerLocation));
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("Distance"), Distance);

}
