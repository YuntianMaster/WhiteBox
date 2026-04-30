// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "EnemyMainWidget.generated.h"

class UDebuffSkillWidget;
UCLASS()
class WHITEBOX_API UEnemyMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta = (BindWidget))
	class UHorizontalBox* DebuffContainer;
	UPROPERTY(BlueprintReadWrite,meta = (ExposeOnSpawn = "TRUE"))
	AActor* EnemyInstigatorActor;
	class UAbilitySystemComponent* ASC;

protected:
	virtual void NativeConstruct() override;


public:
	void UpdateDebuffHandle(const FGameplayTag callback, int32 NewTag);
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UDebuffSkillWidget> DefaultDebuffClass;
};
