// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "QTESystemComp.generated.h"

class UQTE_Base_Widget;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WHITEBOX_API UQTESystemComp : public USceneComponent
{
	GENERATED_BODY()

public:
	UQTESystemComp();

	struct FQTE_PoolActor
	{
		UQTE_Base_Widget* QTE_Widget = nullptr;
		bool bIsValid = true;
	};

protected:
	TArray<FQTE_PoolActor> ActorsPool;

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "QTE_Pool")
	int32 QTE_PoolSize{ 5 };

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "QTE_Pool")
	TSubclassOf<UQTE_Base_Widget> QTE_Sample;

	TArray<UQTE_Base_Widget*> CurrentActivedQTEs;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FQTE_PoolActor* FindFirstQTE_Actor();
	UFUNCTION(BlueprintCallable)
	UQTE_Base_Widget* CreateQTE(float QTE_Time, FName BoneName);
};
