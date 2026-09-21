// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_TakeHit.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Combat/PlayerTraceComponent.h"
#include "AbilitySystemComponent.h"


void UGA_TakeHit::OnTakeHitHandle(const FGameplayEventData& EventData)
{
	AActor* Onwer = GetAvatarActorFromActorInfo();
	UAbilitySystemComponent* OnwerASC =  GetAbilitySystemComponentFromActorInfo();

	FGameplayTag ParryTag = FGameplayTag::RequestGameplayTag("Stats.Parrying");
	FGameplayTag PerfectDodgeTag = FGameplayTag::RequestGameplayTag("Stats.PerfectDodge");
	FGameplayTag DodgeTag = FGameplayTag::RequestGameplayTag("Stats.Dodge");

	if (OnwerASC->HasMatchingGameplayTag(ParryTag))
	{
		UE_LOG(LogTemp, Warning, TEXT("OnTakeHitHandle : Parrying"));
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			Onwer,
			FGameplayTag::RequestGameplayTag("Event.Combat.ParrySuccess"),
			EventData

		);

		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		return;
	}

	if (OnwerASC->HasMatchingGameplayTag(PerfectDodgeTag))
	{
		UE_LOG(LogTemp, Warning, TEXT("OnTakeHitHandle : PerfectDodgeTag"));
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			Onwer,
			FGameplayTag::RequestGameplayTag("Event.Combat.PerfectDodge"),
			EventData

		);
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		return;
	}

	else if (OnwerASC->HasMatchingGameplayTag(DodgeTag))
	{
		UE_LOG(LogTemp, Warning, TEXT("OnTakeHitHandle : DodgeTag"));
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			Onwer,
			FGameplayTag::RequestGameplayTag("Event.Combat.Dodge"),
			EventData

		);
		//UE_LOG(LogTemp, Warning, TEXT("OnTakeHitHandle : DodgeTag: %s"), *EventData.Instigator.GetName());

		if (UPlayerTraceComponent* PTC = EventData.Instigator->GetComponentByClass<UPlayerTraceComponent>())
		{

			PTC->TargetsToIngore.Empty();
			PTC->bIsAttacking = true;
			//UE_LOG(LogTemp, Warning, TEXT("OnTakeHitHandle : DodgeTag: Tracing is retrigger"));
		}


		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("OnTakeHitHandle : HitReaction"));
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		Onwer,
		FGameplayTag::RequestGameplayTag("Event.Combat.HitReaction"),
		EventData

	);
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
