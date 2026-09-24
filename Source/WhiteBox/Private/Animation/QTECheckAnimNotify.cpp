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
	if (!QTE_Sys)
	{
		return;
	}

	// QTE_Time 是玩家真实反应时间。不要乘 SlowRate：
	// 全局时间缩放会把 Timer / Tick 的 Delta 一起放慢，倒计时在 Widget 里改走真实时间。
	UQTE_Base_Widget* QTE_Widget = QTE_Sys->CreateQTE(QTE_Time, BoneName);
	if (!QTE_Widget)
	{
		return;
	}
	
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
