// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/DebuffSkillWidget.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "Components/Image.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTagCountChanged.h"
#include "Components/RadialSlider.h"
#include "Components/TextBlock.h"


void UDebuffSkillWidget::NativeConstruct() {
	Super::NativeConstruct();

}

void UDebuffSkillWidget::UpdateDebuffSkillIconHandle() {

	GetWorld()->GetTimerManager().SetTimer(
		FUpdateIconHandle,
		this,
		&UDebuffSkillWidget::UpdateIcon,
		0.01f,
		true
	);
	ASC->OnGameplayEffectRemoved_InfoDelegate(Handle)->AddUObject(this, &UDebuffSkillWidget::StopUpdateIcon);
}

void UDebuffSkillWidget::UpdateIcon()
{

	const UWorld* World = ASC->GetWorld();
	const float WorldTime = World->GetTimeSeconds();

	BufferStackTimes->SetText(FText::AsNumber(ActiveEffect->Spec.GetStackCount()));
	RemaingTimeSlider->SetValue(ActiveEffect->GetTimeRemaining(WorldTime) / ActiveEffect->GetDuration());
	

}

void UDebuffSkillWidget::StopUpdateIcon(const FGameplayEffectRemovalInfo& RemovalInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("Finish!!"));
}

