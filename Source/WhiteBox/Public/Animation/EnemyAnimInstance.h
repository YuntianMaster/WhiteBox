// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enum/E_Gate.h"
#include "Enum/EPlayerStates.h"
#include "Animation/PlayerAnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UEnemyAnimInstance : public UPlayerAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EPlayerVelocityDirection> ActorDirection{ EPlayerVelocityDirection::VelocityNone };
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EPlayerStates> EnemyStates{ EPlayerStates::CharacterNoneStats };
	UPROPERTY(BlueprintReadWrite)
	FRotator ActorRotation;
	UPROPERTY(BlueprintReadWrite)
	FVector Velocity;
	UPROPERTY(BlueprintReadWrite)
	FVector Velocity2D;
	UPROPERTY(BlueprintReadWrite)
	FVector Acceleration;
	UPROPERTY(BlueprintReadWrite)
	FVector Acceleration2D;

	UPROPERTY(BlueprintReadWrite)
	float LocomotionAngle;
	UPROPERTY(BlueprintReadWrite)
	float DeltaLocation;
	UPROPERTY(BlueprintReadWrite)
	FVector ActorLocation;
	UPROPERTY(BlueprintReadWrite)
	FVector LastFrameActorLocation;

	UPROPERTY(BlueprintReadWrite)
	FVector DestinationLocation;
public:
	UFUNCTION(BlueprintCallable)
	void GetPlayerVelocityDirection();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void UpdateActorProperties();
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	bool bIsBattlePose{ false };
	bool bIsWeaponChanging{ false };
};
