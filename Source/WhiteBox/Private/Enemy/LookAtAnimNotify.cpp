// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/LookAtAnimNotify.h"
#include "Enemy/LookAtPlayerComp.h"

void ULookAtAnimNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	
	if (!IsValid(MeshComp)) return;
	ULookAtPlayerComp* LookComp {MeshComp->GetOwner()->GetComponentByClass<ULookAtPlayerComp>()};
	if (!IsValid(LookComp)) return;
	LookComp->bCanLookAt = false;
}

void ULookAtAnimNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	if (!IsValid(MeshComp)) return;
	ULookAtPlayerComp* LookComp{ MeshComp->GetOwner()->GetComponentByClass<ULookAtPlayerComp>() };
	if (!IsValid(LookComp)) return;
	LookComp->bCanLookAt = true;

}
