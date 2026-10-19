// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CharacterWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UCharacterWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	UPROPERTY()
	class UWidgetHasOwner* StateBarWidget = nullptr;
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TSubclassOf<class UWidgetHasOwner> StateBarWidgetClass;
	
	void ShowStateBarUI();

	void HideStateBarUI();
	
};
