// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "enum/EWeapon.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_EquipWeapon.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API USTT_EquipWeapon : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
	TEnumAsByte<EWeapons> WeaponName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Context")
	class AEnemyCharacter* EnemyActor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WaitEquipTime = 3.f;
protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

	
	
};
