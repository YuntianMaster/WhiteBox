// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Action/BTT_SetMovementSpeed.h"
#include "Enemy/EnemyAIController.h"

EBTNodeResult::Type UBTT_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(AI);

	if (!EnemyAIController) {
		UE_LOG(LogTemp, Warning, TEXT("No EnemyAIController in ChaseToPlayer!"));
		return EBTNodeResult::Failed;
	}

	if (bIsCutstomCombatSpeed) {

		//EnemyAIController->SetEnemyCombatSpeedsByFloat(CustomCombatSpeed);

	}
	else
	{
		//EnemyAIController->SetEnemyCombatSpeedsByEnum(CombatSpeeds);
	}
	return EBTNodeResult::Succeeded;
}
