// Fill out your copyright notice in the Description page of Project Settings.


#include "AttrubuteSet/BasicAttributeSet.h"

#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UBasicAttributeSet::UBasicAttributeSet()
{
	Health = 100.f;
	MaxHealth = 100.f;
	Stamina = 100.f;
	MaxStamina = 100.f;
	Damage = 0.f;
	Shield = 0.f;
	MaxShield = 100.f;
}

void UBasicAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

void UBasicAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
	}
	if (Attribute == GetStaminaAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0, GetMaxStamina());
		UE_LOG(LogTemp, Warning, TEXT("GetStamina:%f"), GetStamina());
	}
	if (Attribute == GetShieldAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0, GetMaxShield());
	}
}


void UBasicAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);


	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		float totalDamage = GetDamage();
		SetDamage(0.f);

		if (GetShield() > 0)
		{
			float RemainingDamage = totalDamage - GetShield();

			//UE_LOG(LogTemp, Warning, TEXT("RemainingDamage:%f"), RemainingDamage);
			if(RemainingDamage>0)
				SetHealth(GetHealth() - RemainingDamage);

		}
		else
			SetHealth(GetHealth() - totalDamage);
	}


	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(GetHealth());

	}

	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute()) {
		SetStamina(GetStamina());
	}
}
