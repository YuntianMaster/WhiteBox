// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enum/EWeapon.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	float CurrentDirect{ 0.0f };
	UPROPERTY(BlueprintReadWrite)
	float CurrentVelocity{ 0.0f };


	UFUNCTION(BlueprintCallable)
	void UpdateTheVelocity();
public:
	UFUNCTION(BlueprintCallable)
	void HandleUpdateTarget(AActor* NewTargetActorRef);
	UFUNCTION(BlueprintCallable)
	void UpdateDirection();
	UPROPERTY(BlueprintReadWrite)
	bool bIsIncombat;
	UPROPERTY(BlueprintReadWrite)
	bool bIsBlocking{ false };

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EWeapons> ECurrentWeapon;
	UPROPERTY(BlueprintReadWrite)
	bool bIsBattlePose{ false };
	UPROPERTY(BlueprintReadWrite)
	bool bIsWeaponChanging{ false };
	UPROPERTY(BlueprintReadWrite)
	bool bIsBowAiming{ false };
	UPROPERTY(BlueprintReadWrite)
	float AimingPitch{ 0.f };
	/*UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);*/


};
	
