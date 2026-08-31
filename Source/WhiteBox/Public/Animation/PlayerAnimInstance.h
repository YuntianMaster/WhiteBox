// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enum/EWeapon.h"
#include "Enum/E_Gate.h"
#include "Enum/EPlayerStates.h"
#include "Structure/FAnimInstStruct.h"
#include "Kismet/KismetMathLibrary.h"
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
	UFUNCTION(BlueprintCallable)
	void UpdateTheVelocity();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void UpdateOrientationData();
	UFUNCTION(BlueprintCallable)
	void UpdateEPlayerVelocityDirection();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void UpdateCharacterGateState();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void UpdateDeltaLocation();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void UpdateAccelerationDirection();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void SetRootYawAngle(float NewRootYawAngle);
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void AccumulateRootYawOffset();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void ProcessTurnYawCurve();

public:
	UPROPERTY(BlueprintReadWrite)
	float CurrentVelocity{ 0.0f };
	UFUNCTION(BlueprintCallable)
	void HandleUpdateTarget(AActor* NewTargetActorRef);
	UFUNCTION(BlueprintCallable)
	void UpdateDirection();
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void UpdateRootYawOffset(float DeltaTime);
	UPROPERTY(BlueprintReadWrite)
	bool bIsIncombat;
	UPROPERTY(BlueprintReadWrite)
	bool bIsBlocking{ false };
	UPROPERTY(BlueprintReadWrite)
	bool bIsGateChange{ false };
	virtual void NativeUpdateAnimation(float DeltaTimeX) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	// Cached on game thread in NativeUpdateAnimation, read on worker thread.
	FVector CachedOwnerVelocity;
	FRotator CachedOwnerRotation;
	bool bCachedIsFalling{ false };
	/*UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EWeapons> ECurrentWeapon;*/
	UPROPERTY(BlueprintReadWrite)
	bool bIsBattlePose{ false };
	UPROPERTY(BlueprintReadWrite)
	bool bIsWeaponChanging{ false };
	UPROPERTY(BlueprintReadWrite)
	bool bIsBowAiming{ false };
	UPROPERTY(BlueprintReadWrite)
	float AimingPitch{ 0.f };
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<E_Gate> CurrentGate;
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<E_Gate> LastFrameGate;
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<E_Gate> IncomingGate;
	UPROPERTY(BlueprintReadWrite)
	float VelocityLocomotionAngle{0.5f};

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EPlayerVelocityDirection> CurrentVelocityDirection { EPlayerVelocityDirection::VelocityNone };

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EPlayerVelocityDirection> CurrentAcelectrationDirection{ EPlayerVelocityDirection::VelocityNone };

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float FrontAngle{ 50.f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BackAngle{ 130.f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DeadZoneAngle{ 20.f };

	FVector LastFrameActorLocation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector CharacterAcceleration;
	UPROPERTY(BlueprintReadWrite)
	float PlayerDeltaLoc;
	UPROPERTY(BlueprintReadWrite)
	float AcelectrationAngle;
	UPROPERTY(BlueprintReadWrite)
	FVector CharacterPivotAcceleration;
	UPROPERTY(BlueprintReadWrite)
	float PlayerYaw;
	UPROPERTY(BlueprintReadWrite)
	float LastFramePlayerYaw;
	UPROPERTY(BlueprintReadWrite)
	float DeltaPlayerYaw;
	UPROPERTY(BlueprintReadWrite)
	float RootYawOffset{ 0.f };
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<E_RootYawOffsetMode> RootYawOffsetMode{ E_RootYawOffsetMode::BlendOut};
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TMap<TEnumAsByte<EWeapons>, TSubclassOf<UAnimInstance>> AnimInstanceLayers;



	FFloatSpringState YawSpringState;
	float LastFrameTurnYawCurveValue{ 0.f };
	float FrameTurnYawCurveValue{ 0.f };
	FString TurnYawCurveName{ "root_rotation_Z" };
	FString IsTurningCurveName{ "IsTurning" };


	//Bone Warping property
	UPROPERTY(BlueprintReadWrite)
	TMap<FName, FAnimInstStruct> BoneWarpMap;
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (BlueprintThreadSafe))
	FAnimInstStruct GetBoneWarpStruct(FName BoneName);



	//Falling Check 
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bShouldPlayFallingAnim = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float FallingEnterFloorDist = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float FallingExitFloorDist = 40.f;

	void UpdateFallingAnimState();
};
	
