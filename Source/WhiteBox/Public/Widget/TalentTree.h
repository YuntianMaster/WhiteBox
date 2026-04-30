// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TalentTree.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UTalentTree : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TArray<class UWBP_Talent*> WBP_Talents;
	
};
