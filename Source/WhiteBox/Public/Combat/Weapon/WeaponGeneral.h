// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enum/EWeapon.h"
#include "Structure/FComboAttackMontages.h"
#include "Structure/FWeaponStruct.h"
#include "Enemy/WeaknessActor.h"
#include "WeaponGeneral.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FReleaseCharingSignature,
	AWeaponGeneral,FReleaseCharingDelegate
);


class UInputMappingConText;
class UInputAction;
struct FInputActionValue;

UCLASS()
class WHITEBOX_API AWeaponGeneral : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AWeaponGeneral();
	UInputMappingConText* DefaultMappingContext;
	APlayerController* ControllerRef;
	class UEnhancedInputLocalPlayerSubsystem* Subsystem;
	class UEnhancedInputComponent* EnhancedInputComp;
	USkeletalMeshComponent* MeshComp;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	ACharacter* PlayerRef;
	class UPlayerAnimInstance* AnimInstance;
	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	virtual float WeaponAttack(UAnimMontage* AttackMontage);
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* EquipMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* UArmMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* BlockMontage;

	UFUNCTION(BlueprintCallable)
	void EquipWeapon();
	UFUNCTION(BlueprintCallable)
	void UArmWeapon();
	virtual void ResertWeapon() {};
	virtual void ActiveWeapon() {};
	UFUNCTION(BlueprintCallable)
	virtual void ChargingWeapon();
	UFUNCTION()
	virtual void ChargingTimeHandler();
	UFUNCTION(BlueprintCallable)
	virtual void ReleaseCharing();
	UFUNCTION(BlueprintCallable)
	virtual void Shooting() {};
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName BackTargetSocket{"Bow_BackTarget"};
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName HandTargetSocket{"bow"};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FWeaponStruct> Weapons;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EWeapons> WeaponName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<USkeletalMeshComponent*> WeaponMeshs;
	FOnMontageEnded FArmedDelegate;
	FOnMontageEnded FUnarmedDelegate;
	FOnMontageEnded FInterruptedDelegate;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FComboAttackMontages> ComboAttackMontages{};
	int ComboGroupIndex{ 0 };
	int ComboCount{ 0 };
	



	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ROLL")
	TArray<UAnimMontage*> Roll_F;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ROLL")
	TArray<UAnimMontage*> Roll_R;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ROLL")
	TArray<UAnimMontage*> Roll_B;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ROLL")
	TArray<UAnimMontage*> Roll_L;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ROLL/TurningAnim")
	UAnimMontage* Turning_R;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ROLL/TurningAnim")
	UAnimMontage* Turning_L;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ROLL")
	UAnimMontage* RollingAttack;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Denfense")
	UAnimMontage* DenfenseAttack;

	UFUNCTION()
	virtual void BlockFuction(){};
	UFUNCTION()
	virtual void CancelBlockFuction() {};
	//Block&Charing
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxCharingTime{ 3.f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MinCharingTime{ 1.f };
	float charingTime{ 0.f };
	FTimerHandle FCharingTimerHandle;
	FReleaseCharingSignature FReleaseCharingDelegate;
	
	//StaminaCost
	class UStatsComponent* StatsComp;
	class IMainPlayerInterface* IplayerRef;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AttackStaminaCost{ 5.0f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CharingStaminaCost{ 0.05f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BlockStaminaCost{ 0.05f };

	
};
