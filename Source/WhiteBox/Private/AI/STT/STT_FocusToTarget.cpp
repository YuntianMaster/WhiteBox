// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STT/STT_FocusToTarget.h"
#include "Enemy/EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/EnemyAIController.h"

EStateTreeRunStatus USTT_FocusToTarget::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	FocusOnTarget(Context);
	return EStateTreeRunStatus::Succeeded;
}

void USTT_FocusToTarget::FocusOnTarget(const FStateTreeExecutionContext& Context)
{
	ActorRef->GetCharacterMovement()->bOrientRotationToMovement = false;
	if(EnemyController->EnemyTargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Focus on Target: %s"), *EnemyController->EnemyTargetActor->GetName());
		EnemyController->SetFocus(EnemyController->EnemyTargetActor, EAIFocusPriority::Gameplay);
	}
}
