// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "DamagePropsWidget.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UDamagePropsWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere, meta = (BindWidget))
	class UProgressBar* ProgressBar_Main;

	void SetPercertage(float Percentage);
};
