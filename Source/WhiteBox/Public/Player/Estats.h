// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum EStats
{
	None UMETA(DisplayName = "No Selected"),
	Health UMETA(DisplayName = "Health"),
	MaxHealth UMETA(DisplayName = "Max Health"),
	Strength UMETA(DisplayName = "Strength"),
	Stamina UMETA(DisplayName = "Stamina"),
	MaxStamina UMETA(DisplayName = "Max Stamina"),
	MeleeRange UMETA(DisplayName = "Melee Range"),
	PatrolWalkSpeed UMETA(DisplayName = "PatrolWalkSpeed"),
	BattleWalkSpeed UMETA(DisplayName = "BattleWalkSpeed"),
	DetectedRate UMETA(DisplayName ="DetectedRate"),
	MaxDetected UMETA(DisplayName = "MaxDetected"),
	Detected UMETA(DisplayName = "Detected")
};
   