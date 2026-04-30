// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffect.h"
#include "DebuffSkillWidget.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UDebuffSkillWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UImage* SkillImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UTextBlock* BufferStackTimes;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta = (BindWidget))
	class URadialSlider* RemaingTimeSlider;
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "TRUE"))
	FActiveGameplayEffectHandle Handle;
	const FActiveGameplayEffect* ActiveEffect;
	class UAbilitySystemComponent* ASC;
	void UpdateDebuffSkillIconHandle();
	UFUNCTION()
	void UpdateIcon();
	void StopUpdateIcon(const FGameplayEffectRemovalInfo& RemovalInfo);
protected:
	
	virtual void NativeConstruct() override;

	FTimerHandle FUpdateIconHandle;
};
