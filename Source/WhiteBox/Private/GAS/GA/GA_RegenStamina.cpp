// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_RegenStamina.h"
#include "AttrubuteSet/BasicAttributeSet.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

void UGA_RegenStamina::GA_RegenStamina()
{

	UE_LOG(LogTemp, Warning, TEXT("Regen START!"));
	RegenSpecHandle = CharRef->AbilitySystemComp->MakeOutgoingSpec(RegenEffect, 1, MakeEffectContext(CurrentSpecHandle, CurrentActorInfo));
	CharRef->AbilitySystemComp->
		GetGameplayAttributeValueChangeDelegate(UBasicAttributeSet::GetStaminaAttribute()).
		AddUObject(this, &UGA_RegenStamina::RegenStaminaHandle);


	
}

void UGA_RegenStamina::RegenStaminaHandle(const FOnAttributeChangeData& Data)
{
	NewValue = Data.NewValue;
	OldValue = Data.OldValue;


	if (OldValue < NewValue)
	{

		return;
	}


	UE_LOG(LogTemp, Warning, TEXT("OldValue1: %f"), OldValue);
	UE_LOG(LogTemp, Warning, TEXT("NewValue1: %f"), NewValue);
	FLatentActionInfo ActionInfo{
		1,
		901,
		TEXT("RegenStaminDataHandle"),
		this
	};

	if(OldValue> NewValue)
	{
		//CharRef->AbilitySystemComp->RemoveActiveGameplayEffect(ActiveHandle, -1);
		UKismetSystemLibrary::RetriggerableDelay(GetWorld(), 2.f, ActionInfo);
	}
}

void UGA_RegenStamina::RegenStaminDataHandle()
{
	UE_LOG(LogTemp, Warning, TEXT("Regen!!!!"));
	if (!RegenEffect) return;
	ActiveHandle =  CharRef->AbilitySystemComp->ApplyGameplayEffectSpecToSelf(*RegenSpecHandle.Data.Get());
}
