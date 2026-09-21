// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/QTECheckAnimNotify.h"
#include "Widget/QTE_Base_Widget.h"
#include "Kismet/GameplayStatics.h"
#include "Player/QTESystemComp.h"

void UQTECheckAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	
	if (!MeshComp || !MeshComp->GetWorld() || !MeshComp->GetWorld()->IsGameWorld())
	{
		return;
	}
	
	UAnimMontage* Montage = Cast<UAnimMontage>(Animation);
	UQTESystemComp* QTE_Sys = MeshComp->GetOwner()->GetComponentByClass<UQTESystemComp>();
	UQTE_Base_Widget* QTE_Widget = QTE_Sys->CreateQTE(QTE_Time* SlowRate,BoneName);
	
	UAnimInstance* AnimInst = MeshComp->GetAnimInstance();
	UGameplayStatics::SetGlobalTimeDilation(MeshComp->GetWorld(), SlowRate);
	QTE_Widget->OnQTEFail.AddLambda([this, AnimInst, MeshComp]()
	{
		AnimInst->Montage_Stop(0.2f);
		UGameplayStatics::SetGlobalTimeDilation(MeshComp->GetWorld(), 1.f);
		
	});

	QTE_Widget->OnQTESuccess.AddLambda([this, Montage, MeshComp]()
		{
			UGameplayStatics::SetGlobalTimeDilation(MeshComp->GetWorld(), 1.f);
		});
}
