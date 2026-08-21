// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Action/BTS_PlayerDistance.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy/EBossStats.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTS_PlayerDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{



	AAIController* const AI = OwnerComp.GetAIOwner();
	UBlackboardComponent* const BB = OwnerComp.GetBlackboardComponent();
	if (!AI || !BB)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BTT_FocusTarget] AI or Blackboard null (AI=%p BB=%p)"), AI, BB);
		return;
	}


	static const FName TargetKey(TEXT("TargetActor"));
	UObject* const TargetObject = BB->GetValueAsObject(TargetKey);
	AActor* const TargetActor = Cast<AActor>(TargetObject);
	if (!TargetActor)
	{

		//UE_LOG(LogTemp, Warning, TEXT("TargetActor is null"));
		return;

	}

	FVector PlayerLocation{ TargetActor->GetActorLocation() };
	FVector EnemyLocation{ OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation() };
	Distance = static_cast<float>(UKismetMathLibrary::Vector_Distance(EnemyLocation, PlayerLocation));
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("Distance"), Distance);

}
