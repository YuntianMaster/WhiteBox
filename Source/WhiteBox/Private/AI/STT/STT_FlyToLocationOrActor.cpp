// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STT/STT_FlyToLocationOrActor.h"
#include "Enemy/EnemyAIController.h"
#include "Enemy/EnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

EStateTreeRunStatus USTT_FlyToLocationOrActor::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)

{
	Super::Tick(Context, DeltaTime);

	const FVector WorldOffset = ToPlayerDirect.ToOrientationRotator().RotateVector(TargetOffset);
	FVector	FinalTargetLocation = WorldOffset + PlayerStartLoc;

	FVector NewLocation = UKismetMathLibrary::VInterpTo_Constant(EnemyCharacter->GetActorLocation(), FinalTargetLocation, GetWorld()->DeltaRealTimeSeconds, MovementSpeed);
	EnemyCharacter->SetActorLocation(NewLocation);
	float Distance = FVector::Distance(EnemyCharacter->GetActorLocation(), FinalTargetLocation);

	if (bIsDebug)
		UKismetSystemLibrary::DrawDebugSphere(
			EnemyCharacter->GetWorld(),
			FinalTargetLocation,
			50.f,
			12,
			FLinearColor::Red,
			1.f,
			5.f);

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
	
	PlayerStartLoc = TargetActor->GetActorLocation();
	EnemyStartLoc = EnemyCharacter->GetActorLocation();

	ToPlayerDirect = (PlayerStartLoc - EnemyStartLoc).GetSafeNormal2D();
	return EStateTreeRunStatus::Running;
}
