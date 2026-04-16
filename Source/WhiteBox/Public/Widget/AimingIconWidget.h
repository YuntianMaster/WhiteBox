// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AimingIconWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FAimingSignature,
	UAimingIconWidget, OnAimingDelegate

);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FShootingSignature,
	UAimingIconWidget, OnShootingDelegate

);
/**
 * 
 */
UCLASS()
class WHITEBOX_API UAimingIconWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FAimingSignature OnAimingDelegate;
	UPROPERTY(BlueprintAssignable)
	FShootingSignature OnShootingDelegate;
};
