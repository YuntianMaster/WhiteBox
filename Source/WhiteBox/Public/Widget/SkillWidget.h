// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemComponent.h"
#include "SkillWidget.generated.h"

class UGA_GameAblilityBase;
/**
 * 
 */
UCLASS()
class WHITEBOX_API USkillWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	FTimerHandle CoolDownHandle;
	UFUNCTION()
	void CoolTimeHandle();
public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class USizeBox* SkillSize;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UOverlay* SkillOverlay;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UImage* SkillImage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UOverlay* CoolDownOverlay;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UImage* CoolDownBG;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UTextBlock* CoolDownTime;
	void CoolDownTagChangeHandle(const FGameplayTag CoolDownTage, int32 TagNum);
	FGameplayAbilitySpecHandle AbilitySpec;



};
