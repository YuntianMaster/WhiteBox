// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Calculation/GAS_Caculation_Shield.h"

float UGAS_Caculation_Shield::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{


	FGameplayTagContainer tags =  GetTargetActorTags(Spec);
	float Damage = GetSetByCallerMagnitudeByTag(Spec, FGameplayTag::RequestGameplayTag("Data.Damage"));
	for (FGameplayTag tag : tags) {


		UE_LOG(LogTemp, Warning, TEXT("TAGS: %s"), *tag.GetTagName().ToString());
		UE_LOG(LogTemp, Warning, TEXT("Damage: %f"), Damage);

	}

	if(tags.HasTag(FGameplayTag::RequestGameplayTag("Stats.Blocking")))
	{
		return Damage/2;
	}
	else {

		return Damage;

	}
}
