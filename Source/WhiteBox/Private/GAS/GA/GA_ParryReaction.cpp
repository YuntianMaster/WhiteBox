// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_ParryReaction.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"

namespace ParryReactionPrivate
{
	UAnimMontage* GetFirstMontage(const TArray<UAnimMontage*>& Anims, const TCHAR* ArrayName)
	{
		if (!Anims.IsValidIndex(0) || !IsValid(Anims[0]))
		{
			UE_LOG(LogTemp, Error, TEXT("UGA_ParryReaction: '%s' is empty or invalid — fill CritcalParryAnim / ParryAnim on the Ability BP"), ArrayName);
			return nullptr;
		}
		return Anims[0];
	}
}

void UGA_ParryReaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_WaitGameplayEvent* ParrySuccessEvent = 
		UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,FGameplayTag::RequestGameplayTag("Event.Combat.ParrySuccess"));
	UAbilityTask_WaitGameplayEvent* CriticalParrySuccessEvent =
		UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("Event.Combat.CriticalParrySuccess"));
	ParrySuccessEvent->EventReceived.AddDynamic(this, &UGA_ParryReaction::ParryHandle);
	CriticalParrySuccessEvent->EventReceived.AddDynamic(this, &UGA_ParryReaction::CriticalParryHandle);
	ParrySuccessEvent->ReadyForActivation();
	CriticalParrySuccessEvent->ReadyForActivation();
}


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
		static_cast<const FGameplayAbilityTargetData_SingleTargetHit*>(Playload.TargetData.Get(1));
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
		if (UAnimMontage* Montage = ParryReactionPrivate::GetFirstMontage(DL_M_ParryAnim, TEXT("DL_M_ParryAnim")))
		{
			ParryReactionMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				"ParryReaction",
				Montage,
				1.f,
				NAME_None,
				true,
				1.f,
				0.f,
				false
			);
		}


	}

	else if (H_Angle360 > 0 && V_Angle < 0) {

		
		UE_LOG(LogTemp, Warning, TEXT("DR"));
		if (UAnimMontage* Montage = ParryReactionPrivate::GetFirstMontage(DR_M_ParryAnim, TEXT("DR_M_ParryAnim")))
		{
			ParryReactionMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				"ParryReaction",
				Montage,
				1.f,
				NAME_None,
				true,
				1.f,
				0.f,
				false
			);
		}


	}

	else if (H_Angle360 > 0 && V_Angle > 0) {

		
		UE_LOG(LogTemp, Warning, TEXT("UR"));
		if (UAnimMontage* Montage = ParryReactionPrivate::GetFirstMontage(UR_M_ParryAnim, TEXT("UR_M_ParryAnim")))
		{
			ParryReactionMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				"ParryReaction",
				Montage,
				1.f,
				NAME_None,
				true,
				1.f,
				0.f,
				false
			);
		}


	}


	else if (H_Angle360 < 0 && V_Angle > 0) {

	
		UE_LOG(LogTemp, Warning, TEXT("UL"));
		if (UAnimMontage* Montage = ParryReactionPrivate::GetFirstMontage(UL_M_ParryAnim, TEXT("UL_M_ParryAnim")))
		{
			ParryReactionMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				"ParryReaction",
				Montage,
				1.f,
				NAME_None,
				true,
				1.f,
				0.f,
				false
			);
		}


	}


	if(ParryReactionMontage)
	{
		ParryReactionMontage->ReadyForActivation();
		ParrySuccessHandle(Playload);
		ParryReactionMontage->OnCompleted.AddDynamic(this, &UGA_ParryReaction::EndAbilityHandler);

	}
	else if (FMath::Abs(H_Angle360) <= ParryAngle)
	{
		EndAbilityHandler();
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

			FGameplayTag MagnitudeTag = FGameplayTag::RequestGameplayTag(FName("Data.Posie"));
		
			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(BeParriedGE, 1.f, Context);

			SpecHandle.Data->SetSetByCallerMagnitude(MagnitudeTag, ParryPoiseMagnitude);
			ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
			/*ASC->ApplyGameplayEffectToTarget(
				BeParriedGE.GetDefaultObject(),
				TargetASC,
				1.f,
				Context
			);*/
		}
	}



	UE_LOG(LogTemp, Warning, TEXT("Playload.Instigator: %s"), *Playload.Instigator.GetName());
	
		
}

void UGA_ParryReaction::CriticalParryHandle(FGameplayEventData Playload)
{

	AActor* Avatar = GetAvatarActorFromActorInfo();
	FVector HitDirection = (Playload.Instigator->GetActorLocation() - Avatar->GetActorLocation()).GetSafeNormal2D();
	FVector ActorForward = Avatar->GetActorForwardVector().GetSafeNormal2D();
	const float ActorYaw = ActorForward.Rotation().Yaw;
	const float HitYaw = HitDirection.Rotation().Yaw;
	const float H_Angle360 = FMath::FindDeltaAngleDegrees(ActorYaw, HitYaw); // -180 ~ 180
	//const float H_Angle360 = FRotator::ClampAxis(H_Angle360);                    // 0 ~ 360
	const FGameplayAbilityTargetData_SingleTargetHit* HitData =
		static_cast<const FGameplayAbilityTargetData_SingleTargetHit*>(Playload.TargetData.Get(1));
	FVector ToHit = (HitData->HitResult.ImpactPoint - Avatar->GetActorLocation()).GetSafeNormal();
	const float V_Angle = FMath::RadiansToDegrees(FMath::Asin(ToHit.Z));



	UAbilityTask_PlayMontageAndWait* ParryReactionMontage = nullptr;

	UE_LOG(LogTemp, Warning, TEXT("H_Angle360: %f"), H_Angle360);
	UE_LOG(LogTemp, Warning, TEXT("V_Angle: %f"), V_Angle);





	if (FMath::Abs(H_Angle360) > ParryAngle)
	{

		FGameplayTag EventAttackHit = FGameplayTag::RequestGameplayTag(FName("Event.Combat.CriticalHitRecation"));

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
		if (UAnimMontage* Montage = ParryReactionPrivate::GetFirstMontage(DL_M_CriticalParryAnim, TEXT("DL_M_CriticalParryAnim")))
		{
			ParryReactionMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				"ParryReaction",
				Montage,
				1.f,
				NAME_None,
				false,
				1.f,
				0.f,
				false
			);
		}


	}

	else if (H_Angle360 > 0 && V_Angle < 0) {


		UE_LOG(LogTemp, Warning, TEXT("DR"));
		if (UAnimMontage* Montage = ParryReactionPrivate::GetFirstMontage(DR_M_CriticalParryAnim, TEXT("DR_M_CriticalParryAnim")))
		{
			ParryReactionMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				"ParryReaction",
				Montage,
				1.f,
				NAME_None,
				false,
				1.f,
				0.f,
				false
			);
		}


	}

	else if (H_Angle360 > 0 && V_Angle > 0) {


		UE_LOG(LogTemp, Warning, TEXT("UR"));
		if (UAnimMontage* Montage = ParryReactionPrivate::GetFirstMontage(UR_M_CriticalParryAnim, TEXT("UR_M_CriticalParryAnim")))
		{
			ParryReactionMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				"ParryReaction",
				Montage,
				1.f,
				NAME_None,
				false,
				1.f,
				0.f,
				false
			);
		}


	}


	else if (H_Angle360 < 0 && V_Angle > 0) {


		UE_LOG(LogTemp, Warning, TEXT("UL"));
		if (UAnimMontage* Montage = ParryReactionPrivate::GetFirstMontage(UL_M_CriticalParryAnim, TEXT("UL_M_CriticalParryAnim")))
		{
			ParryReactionMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				"ParryReaction",
				Montage,
				1.f,
				NAME_None,
				false,
				1.f,
				0.f,
				false
			);
		}


	}


	if (ParryReactionMontage)
	{
		ParryReactionMontage->ReadyForActivation();
		ParrySuccessHandle(Playload);
		ParryReactionMontage->OnCompleted.AddDynamic(this, &UGA_ParryReaction::EndAbilityHandler);

	}
	else if (FMath::Abs(H_Angle360) <= ParryAngle)
	{
		EndAbilityHandler();
	}

}
