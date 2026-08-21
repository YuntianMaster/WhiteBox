// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/EnemyAnimInstance.h"
#include"Enemy/EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UEnemyAnimInstance::GetPlayerVelocityDirection()
{

	//FVector Direction = DestinationLocation - GetOwningActor()->GetActorLocation();
	FRotator Rotation = UKismetMathLibrary::MakeRotFromX(CachedOwnerVelocity);
	Rotation.Yaw = UKismetMathLibrary::NormalizeAxis(Rotation.Yaw);

	if(Rotation.Yaw > -22.5f && Rotation.Yaw <= 22.5f)
	{
		ActorDirection = EPlayerVelocityDirection::VelocityForward;
	}
	else if(Rotation.Yaw > 22.5f && Rotation.Yaw <= 67.5f)
	{
		ActorDirection = EPlayerVelocityDirection::VelocityFR;
	}
	else if(Rotation.Yaw > 67.5f && Rotation.Yaw <= 112.5f)
	{
		ActorDirection = EPlayerVelocityDirection::VelocityRight;
	}
	else if(Rotation.Yaw > 112.5f && Rotation.Yaw <= 157.5f)
	{
		ActorDirection = EPlayerVelocityDirection::VelocityBR;
	}
	else if(Rotation.Yaw > 157.5f || Rotation.Yaw <= -157.5f)
	{
		ActorDirection = EPlayerVelocityDirection::VelocityBackward;
	}
	else if(Rotation.Yaw > -157.5f && Rotation.Yaw <= -112.5f)
	{
		ActorDirection = EPlayerVelocityDirection::VelocityBL;
	}
	else if(Rotation.Yaw > -112.5f && Rotation.Yaw <= -67.5f)
	{
		ActorDirection = EPlayerVelocityDirection::VelocityLeft;
	}
	else if(Rotation.Yaw > -67.5f && Rotation.Yaw <= -22.5f)
	{
		ActorDirection = EPlayerVelocityDirection::VelocityFL;
	}
}

void UEnemyAnimInstance::UpdateActorProperties()
{
	Velocity = GetOwningActor()->GetVelocity();
	GetPlayerVelocityDirection();
	Velocity2D = FVector(Velocity.X, Velocity.Y, 0.0f);
	ActorRotation = GetOwningActor()->GetActorRotation();
	ActorLocation = GetOwningActor()->GetActorLocation();

	DeltaLocation = FVector::Distance(ActorLocation, LastFrameActorLocation);
	LastFrameActorLocation = ActorLocation;

	APawn* PawnOwner = TryGetPawnOwner();
	if(!PawnOwner)
	{
		return;
	}
	UCharacterMovementComponent* CharacterMovementComp = PawnOwner->FindComponentByClass<UCharacterMovementComponent>();
	if (CharacterMovementComp)
	{
		Acceleration = CharacterMovementComp->GetCurrentAcceleration();
		Acceleration2D = FVector(Acceleration.X, Acceleration.Y, 0.0f);
		//UE_LOG(LogTemp, Warning, TEXT("Acceleration: %s"), *Acceleration.ToString());
	};

	EnemyStates = Cast<AEnemyCharacter>(PawnOwner)->EnemyPlayerStats;
	// CurrentGate 由 AEnemyCharacter::SetGate 同步，此处不再赋值
}

void UEnemyAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	UpdateActorProperties();
}


