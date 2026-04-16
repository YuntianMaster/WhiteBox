// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum EWeapons
{
	NoWeapon UMETA(DisplayName = "NoWeapon"),
	GeneralSword UMETA(DisplayName = "GeneralSword"),
	GeneralKatana UMETA(DisplayName = "GeneralKatana"),
	GeneralBow UMETA(DisplayName = "GeneralBow"),
	GeneralQuivler UMETA(DisplayName = "GeneralQuivler"),
	GeneralRock UMETA(DisplayName = "GeneralRock"),
	GeneralDoubleKnife UMETA(DisplayName = "GeneralDoubleKnife")

};