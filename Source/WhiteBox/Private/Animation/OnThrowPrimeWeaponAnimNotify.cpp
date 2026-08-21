// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/OnThrowPrimeWeaponAnimNotify.h"
#include "AbilitySystemBlueprintLibrary.h"


void UOnThrowPrimeWeaponAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	if (!MeshComp || !MeshComp->GetWorld() || !MeshComp->GetWorld()->IsGameWorld())
	{
		return;
	}
	AActor* Owner = MeshComp->GetOwner();
	FGameplayEventData Payload;
	Payload.EventTag = FGameplayTag::RequestGameplayTag("Event.Throw.ThrowPrimeWeapon");
	Payload.Instigator = Owner;
	Payload.Target = Owner;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, Payload.EventTag, Payload);
}
