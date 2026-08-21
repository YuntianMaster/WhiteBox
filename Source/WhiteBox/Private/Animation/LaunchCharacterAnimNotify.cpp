// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/LaunchCharacterAnimNotify.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void ULaunchCharacterAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
	{
		return;
	}

	UWorld* World = MeshComp->GetWorld();
	if (!World || !World->IsGameWorld())
	{
		return; // ????????
	}

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Character)
	{
		return;
	}

	UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
	if (!Movement)
	{
		return;
	}

	const FVector Start = Character->GetActorLocation();
	const FVector WorldOffset = Character->GetActorRotation().RotateVector(LaunchLocation);
	const FVector End = Start + WorldOffset;

	// CustomArc ??????????0????????????????
	const bool bVerticalOnly =
		FMath::IsNearlyZero(WorldOffset.X, 1.f) && FMath::IsNearlyZero(WorldOffset.Y, 1.f);

	if (bVerticalOnly)
	{
		const float H = WorldOffset.Z;
		if (H <= 0.f)
		{
			return;
		}

		const float GravityZ = FMath::Abs(World->GetGravityZ()) * Movement->GravityScale;
		const float Vz = FMath::Sqrt(2.f * GravityZ * H);
		Character->LaunchCharacter(FVector(0.f, 0.f, Vz), true, true);
		return;
	}

	FVector TossVelocity = FVector::ZeroVector;
	const bool bSuccess = UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		MeshComp, TossVelocity, Start, End, 0.f, 0.5f);

	if (bSuccess)
	{
		Character->LaunchCharacter(TossVelocity, true, true);
	}
}
