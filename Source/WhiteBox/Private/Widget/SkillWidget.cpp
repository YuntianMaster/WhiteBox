// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SkillWidget.h"
#include "GameFramework/Pawn.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTagCountChanged.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GAS/GA/GA_GameAblilityBase.h"



void USkillWidget::NativeConstruct()
{
	UE_LOG(LogTemp, Warning, TEXT("skillstart"));

	UAbilitySystemComponent* ASC = GetOwningPlayerPawn()->GetComponentByClass<UAbilitySystemComponent>();
	if (!ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASC is none!"));
		return;

	}
	ASC->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("CoolDown")).AddUObject(this, &USkillWidget::CoolDownTagChangeHandle);
	FText TEX  = FText::FromString(TEXT(""));
	CoolDownTime->SetText(TEX);
	UOverlaySlot* CoolDownBGSlot = Cast<UOverlaySlot>(CoolDownBG->Slot);
	CoolDownBGSlot->SetPadding(FMargin(0.f, 100.f, 0.f, 0.f));
}



void USkillWidget::CoolDownTagChangeHandle(const FGameplayTag CoolDownTage, int32 TagNum)
{
	APawn* OwningPawn = GetOwningPlayerPawn();
	if (!OwningPawn)
	{
		return;
	}

	UAbilitySystemComponent* ASC = OwningPawn->GetComponentByClass<UAbilitySystemComponent>();
	if (!ASC)
	{
		return;
	}

	if (!AbilitySpec.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySpec is invalid"));
		return;
	}

	const FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(AbilitySpec);
	if (!Spec || !Spec->Ability)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySpec handle not found on ASC"));
		return;
	}

	// UE5：GetCooldownTimeRemainingAndDuration 在 UGameplayAbility 上，且必须传入 ASC 的 AbilityActorInfo（无参版 GetCooldownTimeRemaining 在 CDO 上仍为 0）。
	const FGameplayAbilityActorInfo* ActorInfo = ASC->AbilityActorInfo.Get();
	if (!ActorInfo)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASC AbilityActorInfo is null"));
		return;
	}
	float TimeRemaining = 0.f;
	float CooldownDuration = 0.f;
	Spec->Ability->GetCooldownTimeRemainingAndDuration(AbilitySpec, ActorInfo, TimeRemaining, CooldownDuration);
	UE_LOG(LogTemp, Warning, TEXT("Ability: %s"), *Spec->Ability->GetClass()->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Cooldown remaining: %f / duration: %f"), TimeRemaining, CooldownDuration);

	if (TagNum > 0)
	{

		if(TimeRemaining>0)
		{
			GetWorld()->GetTimerManager().SetTimer(
				CoolDownHandle,
				this,
				&USkillWidget::CoolTimeHandle,
				0.1f,
				true
			);
		}
		UE_LOG(LogTemp, Warning, TEXT("tag get, tag num: %d"), TagNum);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(CoolDownHandle);
		CoolDownTime->SetText(FText::FromString(TEXT("")));
		UOverlaySlot* CoolDownBGSlot = Cast<UOverlaySlot>(CoolDownBG->Slot);
		CoolDownBGSlot->SetPadding(FMargin(0.f, 100.f, 0.f, 0.f));
		UE_LOG(LogTemp, Warning, TEXT("tag lost, tag num: %d"), TagNum);
	}
}

void USkillWidget::CoolTimeHandle()
{
	APawn* OwningPawn = GetOwningPlayerPawn();
	if (!OwningPawn)
	{
		return;
	}

	UAbilitySystemComponent* ASC = OwningPawn->GetComponentByClass<UAbilitySystemComponent>();
	if (!ASC)
	{
		return;
	}

	if (!AbilitySpec.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySpec is invalid"));
		return;
	}

	const FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(AbilitySpec);
	if (!Spec || !Spec->Ability)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySpec handle not found on ASC"));
		return;
	}

	// UE5：GetCooldownTimeRemainingAndDuration 在 UGameplayAbility 上，且必须传入 ASC 的 AbilityActorInfo（无参版 GetCooldownTimeRemaining 在 CDO 上仍为 0）。
	const FGameplayAbilityActorInfo* ActorInfo = ASC->AbilityActorInfo.Get();
	if (!ActorInfo)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASC AbilityActorInfo is null"));
		return;
	}
	float TimeRemaining = 0.f;
	float CooldownDuration = 0.f;
	Spec->Ability->GetCooldownTimeRemainingAndDuration(AbilitySpec, ActorInfo, TimeRemaining, CooldownDuration);
	FNumberFormattingOptions FQS;
	FQS.SetMinimumFractionalDigits(1);
	FQS.SetMaximumFractionalDigits(1);
	CoolDownTime->SetText(FText::AsNumber(TimeRemaining, &FQS));

	UOverlaySlot* CoolDownBGSlot = Cast<UOverlaySlot>(CoolDownBG->Slot);
	CoolDownBGSlot->SetPadding(FMargin(0.f, (1 - TimeRemaining / CooldownDuration)*100 , 0.f, 0.f));


}

