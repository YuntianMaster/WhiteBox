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

	FOnMontageEnded FUnArmDelegate;
	FOnMontageEnded FEquipedDelegate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapons")
	TEnumAsByte<EWeapons> ECurrentWeapons;
	TEnumAsByte<EWeapons> EChangeWeapon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapons")
	TEnumAsByte<EWeapons> EUnarmedWeapon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapons/ThrowWeapon")
	AWeaponGeneral* CurrentThrowWeapon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapons/ThrowWeapon")
	TEnumAsByte<EWeapons> ECurrentThrowWeapons;
	TMap<TEnumAsByte<EWeapons>, TArray<AWeaponGeneral*>> AllGaintedWeaponStructs;
	TMap<TEnumAsByte<EWeapons>, UAnimMontage*> WeaponEquipedAnims;
	TMap<TEnumAsByte<EWeapons>, UAnimMontage*> WeaponUnarmAnims;
	UAbilitySystemComponent* Abilitycomp;
	TArray<FGameplayAbilitySpecHandle> WeaponAbility;


	bool bIsChangingWeapon{ false };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void GetWeaponHandle(class UPDA_Weapon* WeaponData);
	UFUNCTION(BlueprintCallable)
	void SwitchWeapon(EWeapons WeaponName);
	void EquipWeaponByEWeapon(EWeapons WeaponName);
	void UArmWeaponByEWeapon(EWeapons WeaponName);

	void EquipWeaponToSocketByEWeapon(EWeapons WeaponName);
	void UnArmWeaponToSocketByEWeapon(EWeapons WeaponName);
	void SetActorToSocket(AActor* Actor, FName SocketName);

	
	UFUNCTION(BlueprintCallable)
	void ArmmedWeaponToSocket(EWeapons WeaponName);
	UFUNCTION(BlueprintCallable)
	void UnArmmedWeaponToSocket(EWeapons WeaponName);
	class UPlayerAnimInstance* AnimInstance;
	//FOnMontageEnded FChangeWeaponDelegate;
	//FOnMontageEnded FArmDelegate;
	UFUNCTION(BlueprintCallable)
	void ThrowWeapon();
	void GiveWeaponAbilitiesByWeaponStruct(FWeaponStruct Weapon);
	void RemoveWeaponAbilitiesByWeaponStruct(FWeaponStruct Weapon);

	UFUNCTION(BlueprintCallable)
	TArray<AWeaponGeneral*> GetWeaponByEWeapon(EWeapons WeaponName);

};



