// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_RollToLocation.h"
#include "Kismet/KismetMathLibrary.h"

EPlayerVelocityDirection UGA_RollToLocation::GetRollDirection()
{
	
	EPlayerVelocityDirection CurrentVelocityDirection;
	FVector ActorLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	FRotator RollRot = UKismetMathLibrary::FindLookAtRotation(ActorLocation, EQSQueryLocation);
	
	RollRot.Yaw = UKismetMathLibrary::NormalizeAxis(RollRot.Yaw - GetAvatarActorFromActorInfo()->GetActorRotation().Yaw);
	float RollAngle = RollRot.Yaw;

	if (RollAngle > -22.5f && RollAngle <= 22.5f)
	{
		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityForward;
	}
	else if (RollAngle > 22.5f && RollAngle <= 67.5f)
	{
		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityFR;
	}
	else if (RollAngle > 67.5f && RollAngle <= 112.5f)
	{
		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityRight;
	}
	else if (RollAngle > 112.5f && RollAngle <= 157.5f)
	{
		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityBR;
	}
	else if (RollAngle > 157.5f || RollAngle <= -157.5f)
	{
		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityBackward;
	}
	else if (RollAngle > -157.5f && RollAngle <= -112.5f)
	{
		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityBL;
	}
	else if (RollAngle > -112.5f && RollAngle <= -67.5f)
	{
		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityLeft;
	}
	else if (RollAngle > -67.5f && RollAngle <= -22.5f)
	{
		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityFL;
	}


	return CurrentVelocityDirection;
	
}

void UGA_RollToLocation::OnEQSFinished(TSharedPtr<FEnvQueryResult> Result)
{
	Super::OnEQSFinished(Result);

	if (!IsActive())          
	{
		UE_LOG(LogTemp, Warning, TEXT("UGA_RollToLocation::OnEQSFinished: Ability End"));
		return;                
	}
	const EPlayerVelocityDirection RollDir = GetRollDirection();
	UE_LOG(LogTemp, Warning, TEXT("[EQS Debug] RollDirection=%d EQSQueryLocation=%s"),
		static_cast<int32>(RollDir),
		*EQSQueryLocation.ToString());
	AttackMontage = RollMontages[RollDir];

	UE_LOG(LogTemp,Warning,TEXT("Roll Montage: %s"), *AttackMontage->GetName())
	WarpMontageHandler();
}
