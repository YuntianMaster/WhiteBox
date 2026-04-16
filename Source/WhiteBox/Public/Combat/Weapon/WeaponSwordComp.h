// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat/Weapon/WeaponGeneral.h"
#include "WeaponSwordComp.generated.h"


UCLASS()
class WHITEBOX_API AWeaponSwordComp : public AWeaponGeneral
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AWeaponSwordComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

		
};
