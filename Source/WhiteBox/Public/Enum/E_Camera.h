// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum E_LockCameraMode {
	LockTarget UMETA(DisplayName = "LockTarget"),
	LockOffsetTarget UMETA(DisplayName = "LockOffsetTarget"),

};
UENUM(BlueprintType)
enum E_BoomArmDir
{
	Left        UMETA(DisplayName = "Left"),
	Right       UMETA(DisplayName = "Right"),
	FrontLeft   UMETA(DisplayName = "FrontLeft"),
	FrontRight  UMETA(DisplayName = "FrontRight"),
	BackLeft    UMETA(DisplayName = "BackLeft"),
	BackRight   UMETA(DisplayName = "BackRight"),
};

UENUM(BlueprintType)
enum E_BoomArmValue
{
	MinBoomLength UMETA(DisplayName = "MinBoomLength"),
	MaxBoomLength UMETA(DisplayName = "MaxBoomLength"),
	BoomHeight UMETA(DisplayName = "BoomHeight"),
	BoomLatera UMETA(DisplayName = "BoomLatera"),
};