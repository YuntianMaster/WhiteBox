// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterWidgetComponent.h"
#include "Widget/WidgetHasOwner.h"

void UCharacterWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		StateBarWidget = UWidgetHasOwner::CreateHasOwnerWidget(World, StateBarWidgetClass, GetOwner());
	}
}

void UCharacterWidgetComponent::ShowStateBarUI()
{
	UE_LOG(LogTemp, Warning, TEXT("UCharacterWidgetComponent::ShowStateBarUI"));
	if (StateBarWidget)
	{
		StateBarWidget->AddToViewport();
	}
}

void UCharacterWidgetComponent::HideStateBarUI()
{
	if (StateBarWidget)
	{
		StateBarWidget->RemoveFromParent();
	}
}
