// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/QTESystemComp.h"
#include "Widget/QTE_Base_Widget.h"
#include "GameFramework/PlayerController.h"

UQTESystemComp::UQTESystemComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UQTESystemComp::BeginPlay()
{
	Super::BeginPlay();

	if (!QTE_Sample)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQTESystemComp::BeginPlay QTE_Sample is not exist!"));
		return;
	}

	APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	for (int32 i = 0; i < QTE_PoolSize; ++i)
	{
		UQTE_Base_Widget* QTE_Widget_Temp = CreateWidget<UQTE_Base_Widget>(PC, QTE_Sample);
		if (!QTE_Widget_Temp)
		{
			continue;
		}

		QTE_Widget_Temp->AddToViewport();
		QTE_Widget_Temp->SetVisibility(ESlateVisibility::Collapsed);

		const int32 PoolIndex = ActorsPool.Add({ QTE_Widget_Temp, true });

		QTE_Widget_Temp->OnQTESuccess.AddLambda([this, PoolIndex]()
		{
			if (ActorsPool.IsValidIndex(PoolIndex))
			{
				ActorsPool[PoolIndex].bIsValid = true;
				CurrentActivedQTEs.Remove(ActorsPool[PoolIndex].QTE_Widget);
				ActorsPool[PoolIndex].QTE_Widget->bIsCurrentQTE = false;
				if (CurrentActivedQTEs.Num() > 0 && CurrentActivedQTEs[0])
				{
					CurrentActivedQTEs[0]->bIsCurrentQTE = true;
				}
			}
		});
		QTE_Widget_Temp->OnQTEFail.AddLambda([this, PoolIndex]()
		{
			if (ActorsPool.IsValidIndex(PoolIndex))
			{
				ActorsPool[PoolIndex].bIsValid = true;
				CurrentActivedQTEs.Remove(ActorsPool[PoolIndex].QTE_Widget);
				ActorsPool[PoolIndex].QTE_Widget->bIsCurrentQTE = false;
				if (CurrentActivedQTEs.Num() > 0 && CurrentActivedQTEs[0])
				{
					CurrentActivedQTEs[0]->bIsCurrentQTE = true;
				}
			}
		});
	}
}

void UQTESystemComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentActivedQTEs.Num() == 0)
	{
		return;
	}

	UQTE_Base_Widget* CurrentQTE = CurrentActivedQTEs[0];
	if (!CurrentQTE || !CurrentQTE->bIsQTEActive)
	{
		return;
	}

	APlayerController* PC = CurrentQTE->GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	// 每帧只判队首一次，同键不会在同一帧打穿后面的 QTE
	if (PC->WasInputKeyJustPressed(CurrentQTE->ExpectedKey))
	{
		CurrentQTE->HandleQTESuccess();
	}
}

UQTESystemComp::FQTE_PoolActor* UQTESystemComp::FindFirstQTE_Actor()
{
	for (FQTE_PoolActor& QTE_Actor : ActorsPool)
	{
		if (QTE_Actor.bIsValid)
		{
			return &QTE_Actor;
		}
	}
	return nullptr;
}

UQTE_Base_Widget* UQTESystemComp::CreateQTE(float QTE_Time, FName BoneName)
{
	FQTE_PoolActor* ActiveQTEActor = FindFirstQTE_Actor();
	if (!ActiveQTEActor || !ActiveQTEActor->QTE_Widget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQTESystemComp::CreateQTE no free QTE in pool"));
		return nullptr;
	}

	UQTE_Base_Widget* NewWidget = ActiveQTEActor->QTE_Widget;
	NewWidget->bIsCurrentQTE = (CurrentActivedQTEs.Num() == 0);
	CurrentActivedQTEs.Add(NewWidget);

	ActiveQTEActor->bIsValid = false;
	NewWidget->CreateQTE(QTE_Time, BoneName);
	return NewWidget;
}
