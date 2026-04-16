// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapon/WeaponGeneral.h"
#include "Enum/EBowStats.h"
#include "BowWeaponGeneral.generated.h"

class AGeneralProjectile;
/**
 * 
 */
UCLASS()
class WHITEBOX_API ABowWeaponGeneral : public AWeaponGeneral
{
	GENERATED_BODY()
public:
	ABowWeaponGeneral();
	virtual void Tick(float DeltaTime) override;
	virtual void BlockFuction() override;
	virtual void CancelBlockFuction() override;
	virtual void ReleaseCharing()override;
	virtual void Shooting()override;
	virtual float WeaponAttack(UAnimMontage* AttackMontage)override;
	class APlayerCharacter* CharRef;
	class UCameraComponent* CameraRef;
	class USpringArmComponent* CamearBoom;
	class UPlayerActionsComponent* ActionRef;
	class UCharacterMovementComponent* MovementRef;
	float initialFOV;
	FVector initialOffset;
	FTimerHandle FNormalToAimHandle;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float AimFOV {60.f};
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector AimOffset{100.f,75.f,45.f};
	FTimerHandle FAimToNormalHandle;
	FTimerHandle FDrawbowHandle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CameraMoveSpeed{ 100.f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UUserWidget> AimIcon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> PlayerArrow;
	UPROPERTY()
	class UAimingIconWidget* AimWidget;
	float NormalSprintSpeed;
	float NormalWalkSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AimWalkSpeed{200.f};
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AimSprintSpeed{ 400.f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* DrawBowMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* Bow_DrawBowMontage;
	AActor* CurrentProjectile;
	virtual void ResertWeapon() override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USoundBase* BowDraw;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USoundBase* BowShoot;
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void CamerFollowToAim();
	UFUNCTION()
	void CamerAimToFollow();
	UFUNCTION(BlueprintCallable)
	AGeneralProjectile* CreateArrow();
	UFUNCTION(BlueprintCallable)
	void GetArrow();
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EBowStats> BowStats{ EBowStats::BowIdle};
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AGeneralProjectile> Arrow;
	
	FOnMontageEnded FAimingDelegate;
	FPlayMontageAnimNotifyDelegate FMontageNotifyDelegate;
	FScriptDelegate FGetArrowDelegate;
	
	bool bIsReadyToFire = false;
	bool isDrawing = false;

	
	


};
