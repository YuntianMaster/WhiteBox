// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Action/BTT_Alarm.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/EEnemyStats.h"
#include "Alarm/AlarmActor.h"



void UBTT_Alarm::StartEnemyAlarm(UBehaviorTreeComponent& OwnerComp)
{
	FVector Location = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(),
		AlarmSound,
		Location
	);


	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAlarmActor::StaticClass(), FoundActors);


	for (AActor* Actor : FoundActors)
	{
		if (AAlarmActor* Alarm = Cast<AAlarmActor>(Actor))
		{
			Alarm->StartAlarm();
		}
	}
	
}

EBTNodeResult::Type UBTT_Alarm::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	StartEnemyAlarm(OwnerComp);
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum("EnemyStats", EEnemyStats::Chase);
	return EBTNodeResult::Succeeded;
}
