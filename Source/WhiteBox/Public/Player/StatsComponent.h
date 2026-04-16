// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/Estats.h"
#include "StatsComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnHealthPercentUpdateSignature,
	UStatsComponent, OnHealthPercentUpdateDelegate,
	float, Percentage
);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnStaminaPercentUpdateSignature,
	UStatsComponent, OnStaminaPercentUpdateDelegate,
	float,Percentage
);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnHealthZeroSignature,
	UStatsComponent,OnHealthZeroDelegate
);


DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnDelectedSignature,
	UStatsComponent, OnDelectedDelegate,
	float,Percentage

);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnHasDelectedSignature,
	UStatsComponent, HasDelectedDelegate
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHITEBOX_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this component's properties
	UStatsComponent();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TMap<TEnumAsByte<EStats>, float> Stats;

	UFUNCTION(BlueprintCallable)
	void ReduceHealth(float Damage, AActor* Opponent);


	UFUNCTION(BlueprintCallable)
	void ReduceStamina(float Amount);

	UFUNCTION(BlueprintCallable)
	void RegenStamina();
	UPROPERTY(VisibleAnywhere)
	bool bCanRegenStamina{ true };
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float StaminaDelayDuration{ 2.0f };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	double StaminaRgenRate{ 100.0f };

	UPROPERTY(BlueprintAssignable)
	FOnHealthPercentUpdateSignature OnHealthPercentUpdateDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnHealthZeroSignature OnHealthZeroDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnStaminaPercentUpdateSignature OnStaminaPercentUpdateDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnDelectedSignature OnDelectedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnHasDelectedSignature HasDelectedDelegate;

	class UBasicAttributeSet* AttributeSet;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION()
	void HandleOnDeath();



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION()
	void EnableRegen();

	UFUNCTION(BlueprintPure)
	float GetStatPercetage(EStats current, EStats Max);


	void UpdateOnDetected();

	void UpdateOnUndetected();

	
		
};
