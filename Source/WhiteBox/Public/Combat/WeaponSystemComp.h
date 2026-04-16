// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enum/EWeapon.h"
#include "Interface/Fighter.h"
#include "Weapon/WeaponGeneral.h"
#include "WeaponSystemComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHITEBOX_API UWeaponSystemComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponSystemComp();
	ACharacter* charRef;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void ChangeWeapon(TSubclassOf<AWeaponGeneral>  WeaponClass);
	UFUNCTION(BlueprintCallable)
	void GetWeapon(TSubclassOf<AWeaponGeneral>  TargetWeapon);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapons")
	TArray<AWeaponGeneral*> WeaponList;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapons")
	TMap<TEnumAsByte<EWeapons>, TSubclassOf <AWeaponGeneral>> WeaponLibrary;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapons")
	AWeaponGeneral* CurrentWeapon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapons")
	AWeaponGeneral* NeedChangeWeapon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapons")
	TEnumAsByte<EWeapons> ECurrentWeapons;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapons/ThrowWeapon")
	AWeaponGeneral* CurrentThrowWeapon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapons/ThrowWeapon")
	TEnumAsByte<EWeapons> ECurrentThrowWeapons;

	
	UFUNCTION(BlueprintCallable)
	//void SetWeaponToSocket(AWeaponGeneral* Weanponclass, FName SocketName);
	void SetWeaponToSocket(USkeletalMeshComponent* Mesh, FName SocketName);
	UFUNCTION(BlueprintCallable)
	void ArmmedWeaponToSocket(AWeaponGeneral* Weanpon);
	UFUNCTION(BlueprintCallable)
	void UnArmmedWeaponToSocket(AWeaponGeneral* Weanpon);
	class UPlayerAnimInstance* AnimInstance;
	UFUNCTION()
	void EquipCurrentWeapon(AWeaponGeneral* TargetWeapon);
	FOnMontageEnded FChangeWeaponDelegate;
	UFUNCTION()
	void UnArmCurrentWeapon();
	class UCombatComponent* CombatRef;
	

	FOnMontageEnded FUnArmDelegate;
	FOnMontageEnded FArmDelegate;

	UFUNCTION(BlueprintCallable)
	void GetThrowWeapons(TSubclassOf<AWeaponGeneral>  TargetWeapon);


	UFUNCTION(BlueprintCallable)
	void ThrowWeapon();
	
	UAbilitySystemComponent* Abilitycomp;
	TArray<FGameplayAbilitySpecHandle> WeaponAbility;
	void GiveWeaponAbilities(AWeaponGeneral* TargetWeapon);
	void RemoveWeaponAbilities(AWeaponGeneral* TargetWeapon);

};



