// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Calculation/GAS_Caculation_Shield.h"
#include "AttrubuteSet/CombatAttributeSet.h"

UGAS_Caculation_Shield::UGAS_Caculation_Shield()
{
	
	StrengthDef.AttributeToCapture = UCombatAttributeSet::GetStrengthAttribute();
	StrengthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	RelevantAttributesToCapture.Add(StrengthDef);


	ArmorDef.AttributeToCapture = UCombatAttributeSet::GetArmorAttribute();
	ArmorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	RelevantAttributesToCapture.Add(ArmorDef);
}

float UGAS_Caculation_Shield::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	
	FAggregatorEvaluateParameters  EvaParam;
	EvaParam.SourceTags = &GetSourceActorTags(Spec);
	EvaParam.TargetTags = &GetTargetActorTags(Spec);
	float StrengthMagnitude = 0;
	GetCapturedAttributeMagnitude(StrengthDef, Spec, EvaParam, StrengthMagnitude);
	float ArmorMagnitude = 0;
	GetCapturedAttributeMagnitude(ArmorDef, Spec, EvaParam, ArmorMagnitude);

	FGameplayTagContainer tags =  GetTargetActorTags(Spec);
	float Damage = 0;
	Damage = GetSetByCallerMagnitudeByTag(Spec, FGameplayTag::RequestGameplayTag("Data.Damage"));
	for (FGameplayTag tag : tags) {


		UE_LOG(LogTemp, Warning, TEXT("TAGS: %s"), *tag.GetTagName().ToString());
		UE_LOG(LogTemp, Warning, TEXT("Damage: %f"), Damage);

	}
	Damage = Damage * (1 - 0.05 * ArmorMagnitude) * (1 + 0.05 * StrengthMagnitude);
	UE_LOG(LogTemp, Warning, TEXT("Final Damage: %f"), Damage);
	if(tags.HasTag(FGameplayTag::RequestGameplayTag("Stats.Blocking")))
	{
		return Damage/2;
	}
	else {

		return Damage;

	}
}
