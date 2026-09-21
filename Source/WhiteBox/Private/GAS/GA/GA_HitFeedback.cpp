// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_HitFeedback.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"



void UGA_HitFeedback::OnHitStopHandler()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

void UGA_HitFeedback::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (bIsHitStop)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), HitStopScale);
		GetWorld()->GetTimerManager().ClearTimer(HitStopHandle);
		GetWorld()->GetTimerManager().SetTimer(
			HitStopHandle,
			this,
			&UGA_HitFeedback::OnHitStopHandler,
			HitStopTime,
			false
		);
	}




	FGameplayCueParameters CueParams;

	CueParams.Instigator = GetAvatarActorFromActorInfo();
	//CueParams.EffectCauser = Cast<AActor>(Playload.Instigator);
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	//ASC->ExecuteGameplayCue(
	//	GamePlayCueTag,
	//	CueParams
	//);



}

void UGA_HitFeedback::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	Super::EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
