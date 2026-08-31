// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/GameplayAbility.h"
#include "AbilityTask_Tick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTick, float, DeltaTime);


/**
 * 
 */
UCLASS()
class WHITEBOX_API UAbilityTask_Tick : public UAbilityTask
{
	GENERATED_BODY()

public:
    
    UPROPERTY(BlueprintAssignable)
    FOnTick OnTick;
    static UAbilityTask_Tick* Create(UGameplayAbility* OwningAbility) {

        UAbilityTask_Tick* Task = NewAbilityTask<UAbilityTask_Tick>(OwningAbility);
        return Task;
  
    }
protected:
    virtual void Activate() override
    {
        bTickingTask = true;  // ¹Ø¼ü£º¿ªÆô Task Tick
        Super::Activate();
    }
    virtual void TickTask(float DeltaTime) override
    {
        Super::TickTask(DeltaTime);
        OnTick.Broadcast(DeltaTime);
    }
};
