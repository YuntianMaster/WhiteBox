// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/Evalutor/CameraAngleReset.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"


void UCameraAngleReset::GetAngleBetweenCameraAndTarget()
{

	UActorComponent* Comp = Cast<UActorComponent>(EvaluationContextOwner);
	APlayerCharacter* OnwerActor = Cast<APlayerCharacter>(Comp->GetOwner());
	if(!OnwerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner actor is not a player character!"));
		return;
	}
	if(!OnwerActor->TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target actor is not set!"));
		return;
	}
	AActor* TargetActor = OnwerActor->TargetActor;
	FVector CameraDirection = CameraPose.Location - TargetActor->GetActorLocation();
	FVector PlayerDirection = OnwerActor->GetActorLocation() - TargetActor->GetActorLocation();

	double dot = CameraDirection.CosineAngle2D(PlayerDirection);

	const float AngleDeg = FMath::RadiansToDegrees(
		FMath::Acos(FMath::Clamp(dot, -1.f, 1.f)));

	FVector end = CameraDirection.RotateAngleAxis(AngleDeg, FVector::UpVector);


	UE_LOG(LogTemp, Warning, TEXT("CameraDirection: %s, PlayerDirection: %s, Dot: %f"), 
		*CameraDirection.ToString(), 
		*PlayerDirection.ToString(), dot);

	UKismetSystemLibrary::DrawDebugLine(
		this,
		FVector(CameraPose.Location.X, CameraPose.Location.Y,0),
		FVector(TargetActor->GetActorLocation().X, TargetActor->GetActorLocation().Y,0),
		FLinearColor::Red,
		0.5f,
		2.0f
	);
	UKismetSystemLibrary::DrawDebugLine(
		this,
		FVector(OnwerActor->GetActorLocation().X, OnwerActor->GetActorLocation().Y,0),
		FVector(TargetActor->GetActorLocation().X, TargetActor->GetActorLocation().Y,0),
		FLinearColor::Blue,
		0.5f,
		2.0f
	);

	UKismetSystemLibrary::DrawDebugLine(
		this,
		FVector(TargetActor->GetActorLocation().X, TargetActor->GetActorLocation().Y, 0),
		FVector(TargetActor->GetActorLocation().X, TargetActor->GetActorLocation().Y, 0) + end * 500,
		FLinearColor::Green,
		0.5f,
		2.0f
	);
	
}
