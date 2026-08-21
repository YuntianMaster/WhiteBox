// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_HitReaction.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void UGA_HitReaction::GetHit(FVector HitLocation)
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	FVector HitDirection = (HitLocation - Avatar->GetActorLocation()).GetSafeNormal2D();
	FVector ActorForward = Avatar->GetActorForwardVector().GetSafeNormal2D();
	const float ActorYaw = ActorForward.Rotation().Yaw;
	const float HitYaw = HitDirection.Rotation().Yaw;
	const float DeltaSigned = FMath::FindDeltaAngleDegrees(ActorYaw, HitYaw); // -180 ~ 180
	const float Angle360 = FRotator::ClampAxis(DeltaSigned);                    // 0 ~ 360

	
	UAbilityTask_PlayMontageAndWait* HitReactionMontage = nullptr;

	if (Angle360 < 30)
	{
		HitReactionMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"HitReaction",
			M_HitReaction_F,
			1,
			NAME_None,
			true,
			1,
			0,
			false
		);
	}


	else if (Angle360 < 60)
	{
		HitReactionMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"HitReaction",
			M_HitReaction_FR,
			1,
			NAME_None,
			true,
			1,
			0,
			false
		);
	}



	else if (Angle360 < 135)
	{
		HitReactionMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"HitReaction",
			M_HitReaction_R,
			1,
			NAME_None,
			true,
			1,
			0,
			false
		);
	}

	else if (Angle360 < 225)
	{
		HitReactionMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"HitReaction",
			M_HitReaction_B,
			1,
			NAME_None,
			true,
			1,
			0,
			false
		);
	}

	else if (Angle360 < 300)
	{
		HitReactionMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"HitReaction",
			M_HitReaction_L,
			1,
			NAME_None,
			true,
			1,
			0,
			false
		);
	}

	else if (Angle360 < 330)
	{
		HitReactionMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"HitReaction",
			M_HitReaction_FL,
			1,
			NAME_None,
			true,
			1,
			0,
			false
		);
	}

	else {
		HitReactionMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"HitReaction",
			M_HitReaction_F,
			1,
			NAME_None,
			true,
			1,
			0,
			false
		);
	}

	HitReactionMontage->OnCompleted.AddDynamic(this, &UGA_HitReaction::OnHitReactionMontageCompleted);
	HitReactionMontage->OnCancelled.AddDynamic(this, &UGA_HitReaction::OnHitReactionMontageCompleted);
	HitReactionMontage->OnBlendOut.AddDynamic(this, &UGA_HitReaction::OnHitReactionMontageCompleted);
	HitReactionMontage->OnInterrupted.AddDynamic(this, &UGA_HitReaction::OnHitReactionMontageCompleted);
	HitReactionMontage->ReadyForActivation();
}

void UGA_HitReaction::OnHitReactionMontageCompleted()
{

	UE_LOG(LogTemp, Warning, TEXT("END HITREACION"));
	EndAbility(
		GetCurrentAbilitySpecHandle(),
		GetCurrentActorInfo(),
		GetCurrentActivationInfo(),
		true,
		false
	);
}
