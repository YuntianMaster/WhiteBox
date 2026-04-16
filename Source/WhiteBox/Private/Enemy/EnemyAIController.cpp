// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/EEnemyStats.h"
#include "Perception/AISense_Sight.h"

void AEnemyAIController::isSeeingPlayer(const FAIStimulus& Stimulus)
{
	TSubclassOf<UAISense> SenseClass = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);
	if (SenseClass != UAISense_Sight::StaticClass())return;
	if (!Stimulus.WasSuccessfullySensed())return;
	UBlackboardComponent* BlackBoardComp  = GetBlackboardComponent();
	BlackBoardComp->SetValueAsEnum("EnemyStats", EEnemyStats::Alarm);
}
