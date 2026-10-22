// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_RegenGrayHP.h"
#include "AttrubuteSet/BasicAttributeSet.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitAttributeChange.h"

void UGA_RegenGrayHP::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
	RegenGrayHPEffectHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		RegenGrayHPEffect,
		GetAbilityLevel(),
		MakeEffectContext(CurrentSpecHandle, CurrentActorInfo)
	);

	GetAbilitySystemComponentFromActorInfo()->GetGameplayAttributeValueChangeDelegate(UBasicAttributeSet::GetHealthAttribute()).
		AddUObject(this, &UGA_RegenGrayHP::RegenGrayHPHandle);

}

void UGA_RegenGrayHP::RegenGrayHPHandle(const FOnAttributeChangeData& Data)
{
	float NewValue = Data.NewValue;
	float OldValue = Data.OldValue;

	if (FMath::IsNearlyEqual(NewValue, OldValue))
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("UGA_RegenGrayHP::RegenGrayHPHandle: NewValue: %f , OldValue: %f"), NewValue, OldValue);

	FLatentActionInfo RegenGrayHPInfo{
		1,
		902,
		TEXT("RegenGrayHPDataHandle"),
		this
	};


	if (NewValue < OldValue)
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(ActiveRegenGrayHPEffectHandle,-1);
		UKismetSystemLibrary::RetriggerableDelay(GetWorld(), DelayRecoveyGrayHPTime, RegenGrayHPInfo);
	}

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	const UBasicAttributeSet* BasicAtr = ASC ? Cast<UBasicAttributeSet>(ASC->GetAttributeSet(UBasicAttributeSet::StaticClass())) : nullptr;
	if (!BasicAtr)
	{
		return;
	}

	float CurrentGayHP = BasicAtr->GetGrayHealth();
	if (NewValue >= CurrentGayHP) {

		GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(ActiveRegenGrayHPEffectHandle, -1);
		ActiveRegenGrayHPEffectHandle.Invalidate();
	}

}

void UGA_RegenGrayHP::RegenGrayHPDataHandle()
{

	UE_LOG(LogTemp, Warning, TEXT("UGA_RegenGrayHP::RegenGrayHPDataHandle"));
	if (!RegenGrayHPEffect || !RegenGrayHPEffectHandle.IsValid())
	{
		return;
	}
	ActiveRegenGrayHPEffectHandle = GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*RegenGrayHPEffectHandle.Data.Get());

}
