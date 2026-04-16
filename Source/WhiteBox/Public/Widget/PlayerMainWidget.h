// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMainWidget.generated.h"

/**
 * 
 */

class USkillWidget;
UCLASS()
class WHITEBOX_API UPlayerMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UHorizontalBox* SkillContainer;
	void UpdatePlayerHUD();
protected:
	USkillWidget* TryCreateSkillUI();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<USkillWidget> DefaultSkillClass;
	
};
