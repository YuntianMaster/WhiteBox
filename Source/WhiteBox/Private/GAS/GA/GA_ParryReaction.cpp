// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_ParryReaction.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "AbilitySystemBlueprintLibrary.h"


void UGA_ParryReaction::ParryHandle(FGameplayEventData Playload)
{

	AActor* Avatar = GetAvatarActorFromActorInfo();
	FVector HitDirection = (Playload.Instigator->GetActorLocation() - Avatar->GetActorLocation()).GetSafeNormal2D();
	FVector ActorForward = Avatar->GetActorForwardVector().GetSafeNormal2D();
	const float ActorYaw = ActorForward.Rotation().Yaw;
	const float HitYaw = HitDirection.Rotation().Yaw;
	const float H_Angle360 = FMath::FindDeltaAngleDegrees(ActorYaw, HitYaw); // -180 ~ 180
	//const float H_Angle360 = FRotator::ClampAxis(H_Angle360);                    // 0 ~ 360
	const FGameplayAbilityTargetData_SingleTargetHit* HitData =
		static_cast<const FGameplayAbilityTargetData_SingleTargetHit*>(Playload.TargetData.Get(0));
	FVector ToHit = (HitData->HitResult.ImpactPoint - Avatar->GetActorLocation()).GetSafeNormal();
	const float V_Angle = FMath::RadiansToDegrees(FMath::Asin(ToHit.Z));

	

	UAbilityTask_PlayMontageAndWait* ParryReactionMontage = nullptr;

	UE_LOG(LogTemp, Warning, TEXT("H_Angle360: %f"), H_Angle360);
	UE_LOG(LogTemp, Warning, TEXT("V_Angle: %f"), V_Angle);


	

	if (FMath::Abs(H_Angle360) > ParryAngle)
	{ 

		FGameplayTag EventAttackHit = FGameplayTag::RequestGameplayTag(FName("Event.Combat.HitReaction"));
		
		FGameplayEventData Playload_Defender;
		FGameplayAbilityTargetDataHandle DenfenderData;
		Playload_Defender.EventTag = EventAttackHit;
		Playload_Defender.Instigator = Playload.Instigator;

		UE_LOG(LogTemp, Warning, TEXT("Playload.Instigator:: %s"), *Playload.Instigator.GetName());
		Playload_Defender.Target = Avatar;
		UE_LOG(LogTemp, Warning, TEXT("Playload_Defender.Target: %s"), *Playload_Defender.Target.GetName());
		FGameplayAbilityTargetData_SingleTargetHit* const SingleDefendData = new FGameplayAbilityTargetData_SingleTargetHit();
		SingleDefendData->HitResult = HitData->HitResult;
		DenfenderData.Add(SingleDefendData);
		Playload_Defender.TargetData = DenfenderData;






		Playload.EventTag = EventAttackHit;
		UE_LOG(LogTemp, Warning, TEXT("BACK ATTACKING"));
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			Avatar,
			EventAttackHit,
			Playload_Defender);
		EndAbility(
			GetCurrentAbilitySpecHandle(),
			GetCurrentActorInfo(),
			GetCurrentActivationInfo(),
			true,
			false);
	}

	else if (H_Angle360 < 0 && V_Angle < 0) {

		

		UE_LOG(LogTemp, Warning, TEXT("DL"));
		ParryReactionMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"ParryReaction",
			DL_M_ParryAnim[0],
			1.f,
			NAME_None,
			true,
			1.f,
			0.f,
			false
		);


	}

	else if (H_Angle360 > 0 && V_Angle < 0) {

		
		UE_LOG(LogTemp, Warning, TEXT("DR"));
		ParryReactionMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"ParryReaction",
			DR_M_ParryAnim[0],
			1.f,
			NAME_None,
			true,
			1.f,
			0.f,
			false
		);


	}

	else if (H_Angle360 > 0 && V_Angle > 0) {

		
		UE_LOG(LogTemp, Warning, TEXT("UR"));
		ParryReactionMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"ParryReaction",
			UR_M_ParryAnim[0],
			1.f,
			NAME_None,
			true,
			1.f,
			0.f,
			false
		);


	}


	else if (H_Angle360 < 0 && V_Angle > 0) {

	
		UE_LOG(LogTemp, Warning, TEXT("UL"));
		ParryReactionMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"ParryReaction",
			UL_M_ParryAnim[0],
			1.f,
			NAME_None,
			true,
			1.f,
			0.f,
			false
		);


	}


	if(ParryReactionMontage)
	{
		ParryReactionMontage->ReadyForActivation();
		ParrySuccessHandle(Playload);
		ParryReactionMontage->OnCompleted.AddDynamic(this, &UGA_ParryReaction::EndAbilityHandler);

	}
}


void UGA_ParryReaction::EndAbilityHandler()
{

	EndAbility(
		GetCurrentAbilitySpecHandle(),
		GetCurrentActorInfo(),
		GetCurrentActivationInfo(),
		true,
		false);
}

void UGA_ParryReaction::ParrySuccessHandle(FGameplayEventData Playload)
{
	const FGameplayAbilityTargetData_SingleTargetHit* HitData =
		static_cast<const FGameplayAbilityTargetData_SingleTargetHit*>(Playload.TargetData.Get(1));


	FGameplayCueParameters CueParams;
	CueParams.Location = HitData->HitResult.ImpactPoint;
	CueParams.Normal = HitData->HitResult.ImpactNormal;
	CueParams.Instigator = GetAvatarActorFromActorInfo();
	//CueParams.EffectCauser = Cast<AActor>(Playload.Instigator);
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	ASC->ExecuteGameplayCue(
		FGameplayTag::RequestGameplayTag(FName("GameplayCue.Combat.Hit.MetalEffect")),
		CueParams
	);
	
	
	
	if (BeParriedGE && Playload.Instigator)
	{
		UAbilitySystemComponent* TargetASC =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(const_cast<AActor*>(Playload.Instigator.Get()));
		if (TargetASC)
		{
			FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
			Context.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
		
			ASC->ApplyGameplayEffectToTarget(
				BeParriedGE.GetDefaultObject(),
				TargetASC,
				1.f,
				Context
			);
		}
	}



	UE_LOG(LogTemp, Warning, TEXT("Playload.Instigator: %s"), *Playload.Instigator.GetName());
	
		
}
