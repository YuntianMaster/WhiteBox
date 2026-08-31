// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STT/STT_ClearFocus.h"
#include "Enemy/EnemyAIController.h"
#include "Enemy/EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"

EStateTreeRunStatus USTT_ClearFocus::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	CancelFocusTarget();
	return EStateTreeRunStatus::Succeeded;
}

void USTT_ClearFocus::CancelFocusTarget()
{
	if (ActorRef)
	{
		ActorRef->GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	if (!EnemyController)
	{
		return;
	}

	APawn* Pawn = EnemyController->GetPawn();
	const FRotator KeepRotation = Pawn
		? Pawn->GetActorRotation()
		: EnemyController->GetControlRotation();


	
	// SetFocus(Gameplay) 会把 Priorities 扩到 3 格；Default/Move 默认 Position 是 (0,0,0)。
	// 只清 Gameplay 时 GetFocalPoint 会落到原点，角色就会逐渐转向 0,0,0。
	EnemyController->ClearFocus(EAIFocusPriority::Gameplay);
	EnemyController->ClearFocus(EAIFocusPriority::Move);
	EnemyController->ClearFocus(EAIFocusPriority::Default);

	EnemyController->SetControlRotation(KeepRotation);
	if (Pawn)
	{
		Pawn->SetActorRotation(FRotator(0.f, KeepRotation.Yaw, 0.f));
	}
}
