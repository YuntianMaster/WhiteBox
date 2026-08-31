// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STT/STT_FocusToTarget.h"
#include "Enemy/EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/EnemyAIController.h"
#include "Kismet/KismetMathLibrary.h"

EStateTreeRunStatus USTT_FocusToTarget::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	FocusOnTarget();

	if (!EnemyController || !EnemyController->EnemyTargetActor || !ActorRef)
	{
		return EStateTreeRunStatus::Failed;
	}

	if (!bWaitUntilFacing || IsFacingTarget())
	{
		return EStateTreeRunStatus::Succeeded;
	}

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTT_FocusToTarget::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);

	if (!bWaitUntilFacing)
	{
		return EStateTreeRunStatus::Succeeded;
	}

	if (!EnemyController || !EnemyController->EnemyTargetActor || !ActorRef)
	{
		return EStateTreeRunStatus::Failed;
	}

	return IsFacingTarget() ? EStateTreeRunStatus::Succeeded : EStateTreeRunStatus::Running;
}

void USTT_FocusToTarget::FocusOnTarget()
{
	if (!ActorRef || !EnemyController)
	{
		return;
	}

	ActorRef->GetCharacterMovement()->bOrientRotationToMovement = false;
	if (EnemyController->EnemyTargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Focus on Target: %s"), *EnemyController->EnemyTargetActor->GetName());
		EnemyController->SetFocus(EnemyController->EnemyTargetActor, EAIFocusPriority::Gameplay);
	}
}

bool USTT_FocusToTarget::IsFacingTarget() const
{
	const AActor* Target = EnemyController ? EnemyController->EnemyTargetActor : nullptr;
	if (!ActorRef || !Target)
	{
		return false;
	}

	const float DesiredYaw = UKismetMathLibrary::FindLookAtRotation(
		ActorRef->GetActorLocation(),
		Target->GetActorLocation()).Yaw;
	const float YawDelta = FMath::Abs(UKismetMathLibrary::NormalizeAxis(
		DesiredYaw - ActorRef->GetActorRotation().Yaw));

	return YawDelta <= AcceptanceAngle;
}
