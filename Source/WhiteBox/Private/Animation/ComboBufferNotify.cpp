// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ComboBufferNotify.h"
#include "Player/PlayerCharacter.h"


void UComboBufferNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	/*if (!MeshComp) return;
	APlayerCharacter* CharRef{ MeshComp->GetOwner<APlayerCharacter>() };
	if (!CharRef) return;
	CharRef->CombatComp->bCanComboInput = true;*/
}

void UComboBufferNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	/*if (!MeshComp) return;
	APlayerCharacter* CharRef{ MeshComp->GetOwner<APlayerCharacter>() };
	if (!CharRef) return;
	CharRef->CombatComp->bCanComboInput = false;
	CharRef->CombatComp->ComboCloseHandle();*/
}
