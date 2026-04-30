// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WBP_Talent.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UWBP_Talent : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UImage* TalentIcon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UTextBlock* TalentName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UTextBlock* TalentDescription;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UTextBlock* TalentLevel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UButton* ComfirmButton;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UPDA_Talent* TalentData;
protected:
	virtual void NativeConstruct() override;
public:
	UFUNCTION()
	void UpdateUI();
	void InitialUI();
	UFUNCTION()
	void LevelUpTalent();

};
