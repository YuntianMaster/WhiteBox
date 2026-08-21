// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/OnThrowSecondWeponAnimNotify.h"
#include "AbilitySystemBlueprintLibrary.h"

void UOnThrowSecondWeponAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp || !MeshComp->GetWorld() || !MeshComp->GetWorld()->IsGameWorld())
	{
		return;
	}
	AActor* Owner = MeshComp->GetOwner();
	FGameplayEventData Payload;
	Payload.EventTag = FGameplayTag::RequestGameplayTag("Event.Throw.ThrowSecondWeapon");
	Payload.Instigator = Owner;
	Payload.Target = Owner;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, Payload.EventTag, Payload);

}
