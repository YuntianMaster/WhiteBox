// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enum/E_Gate.h"
#include "Animation/EnemyAnimInstance.h"
#include "Structure/FMovementAnimStruct.h"
#include "AnimInstanceLayersBase.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UAnimInstanceLayersBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TMap<TEnumAsByte<E_Gate>, FAnimStateAnimStruct> MovementAnimCollection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequenceBase* IdleAnim;


public:
	virtual void PostInitProperties() override;
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UAnimSequenceBase* GetMovementAnim(
		TEnumAsByte<E_Gate> Gate,
		TEnumAsByte<EAnimState> AnimState,
		TEnumAsByte<EPlayerVelocityDirection> Direction) const;
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UEnemyAnimInstance* GetEnemyAnimInstance() const;

};
