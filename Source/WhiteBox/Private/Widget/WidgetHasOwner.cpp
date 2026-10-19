// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetHasOwner.h"
#include "AbilitySystemComponent.h"
#include "AttrubuteSet/BasicAttributeSet.h"
#include "Abilities/Async/AbilityAsync.h"
#include "Abilities/Async/AbilityAsync_WaitAttributeChanged.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Components/ProgressBar.h"

UWidgetHasOwner* UWidgetHasOwner::CreateHasOwnerWidget(UWorld* World, TSubclassOf<UWidgetHasOwner> WidgetClass, AActor* InWidgetOwnerActor)
{
	if (!World || !WidgetClass || !InWidgetOwnerActor)
	{
		return nullptr;
	}

	UWidgetHasOwner* Widget = CreateWidget<UWidgetHasOwner>(World, WidgetClass);
	if (Widget)
	{
		Widget->WidgetOwnerActor = InWidgetOwnerActor;
	}
	return Widget;
}

void UWidgetHasOwner::UpdateHP(FGameplayAttribute Attribute, float NewValue, float OldValue)
{
	
	const UBasicAttributeSet* Attributes = ASC ? Cast<UBasicAttributeSet>(ASC->GetAttributeSet(UBasicAttributeSet::StaticClass())) : nullptr;
	if (!Attributes || !HP_ProgressBar)
	{
		return;
	}

	const float MaxHP = Attributes->GetMaxHealth();
	if (MaxHP <= 0.f)
	{
		return;
	}

	HP_ProgressBar->SetPercent(Attributes->GetHealth() / MaxHP);
}

void UWidgetHasOwner::NativeConstruct()
{
	Super::NativeConstruct();

	if (WidgetOwnerActor)
	{
		ASC = WidgetOwnerActor->FindComponentByClass<UAbilitySystemComponent>();
	}

	HPAttributeChangeTask = UAbilityAsync_WaitAttributeChanged::WaitForAttributeChanged(
		WidgetOwnerActor,
		UBasicAttributeSet::GetHealthAttribute(),
		false);
	if (!HPAttributeChangeTask)
	{
		return;
	}

	const UBasicAttributeSet* Attributes = ASC ? Cast<UBasicAttributeSet>(ASC->GetAttributeSet(UBasicAttributeSet::StaticClass())) : nullptr;
	if (!Attributes || !HP_ProgressBar)
	{
		return;
	}

	const float MaxHP = Attributes->GetMaxHealth();
	if (MaxHP <= 0.f)
	{
		return;
	}

	HP_ProgressBar->SetPercent(Attributes->GetHealth() / MaxHP);

	HPAttributeChangeTask->Changed.AddDynamic(this, &UWidgetHasOwner::UpdateHP);

	UBlueprintAsyncActionBase* AsyncAction = HPAttributeChangeTask;
	AsyncAction->RegisterWithGameInstance(this);
	AsyncAction->Activate();
}

void UWidgetHasOwner::NativeDestruct()
{
	if (HPAttributeChangeTask)
	{
		UAbilityAsync* AsyncAction = HPAttributeChangeTask;
		AsyncAction->EndAction();
		HPAttributeChangeTask = nullptr;
	}

	Super::NativeDestruct();
}
