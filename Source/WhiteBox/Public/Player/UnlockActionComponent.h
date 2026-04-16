// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnlockActionComponent.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputMappingContext;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHITEBOX_API UUnlockActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUnlockActionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void LockInputMovement(FName reason);
	void UnlockInputMovement(FName reason);
	void ApplyLock();
	void ApplyUnlock();
	UPROPERTY(BlueprintReadWrite)
	TSet<FName> LockReason;

	UEnhancedInputLocalPlayerSubsystem* InputSystem;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UInputMappingContext* DefaultContentMapping;
};
