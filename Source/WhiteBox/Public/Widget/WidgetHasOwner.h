// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttributeSet.h"

class UAbilityAsync_WaitAttributeChanged;

#include "WidgetHasOwner.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UWidgetHasOwner : public UUserWidget
{
	GENERATED_BODY()
public:
	static UWidgetHasOwner* CreateHasOwnerWidget(UWorld* World, TSubclassOf<UWidgetHasOwner> WidgetClass, AActor* InWidgetOwnerActor);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "TRUE"))
	AActor* WidgetOwnerActor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UProgressBar* HP_ProgressBar;

	UPROPERTY()
	class UAbilitySystemComponent* ASC;

	UPROPERTY()
	class UAbilityAsync_WaitAttributeChanged* HPAttributeChangeTask;

	UFUNCTION()
	void UpdateHP(FGameplayAttribute Attribute, float NewValue, float OldValue);
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
