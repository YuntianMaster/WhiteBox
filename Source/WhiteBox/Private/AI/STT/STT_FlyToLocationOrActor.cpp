// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STT/STT_FlyToLocationOrActor.h"
#include "Enemy/EnemyAIController.h"
#include "Enemy/EnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

EStateTreeRunStatus USTT_FlyToLocationOrActor::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)

{
	Super::Tick(Context, DeltaTime);
	FVector NewLocation = UKismetMathLibrary::VInterpTo_Constant(EnemyCharacter->GetActorLocation(), TargetActor->GetActorLocation(), GetWorld()->DeltaRealTimeSeconds, MovementSpeed);
	EnemyCharacter->SetActorLocation(NewLocation);
	float Distance = FVector::Distance(EnemyCharacter->GetActorLocation(), TargetActor->GetActorLocation());
	if(Distance>Radius)
		return EStateTreeRunStatus::Running;
	else
	{
		EnemyCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		return EStateTreeRunStatus::Succeeded;
	}
}

EStateTreeRunStatus USTT_FlyToLocationOrActor::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	TargetActor = EnemyAIController->EnemyTargetActor;
	return EStateTreeRunStatus::Running;
}
