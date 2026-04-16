// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)

enum PlayerDirectionInput
{
	F_0 UMETA(DisplayName = "F_0"),
	F_Right45 UMETA(DisplayName = "F_Right45"),
	F_Right90 UMETA(DisplayName = "F_Right90"),
	B_Right45 UMETA(DisplayName = "B_Right45"),
	F_Left45 UMETA(DisplayName = "F_Left45"),
	F_Left90 UMETA(DisplayName = "F_Left90"),
	B_Left45 UMETA(DisplayName = "B_Left45"),
	B_180 UMETA(DisplayName = "B_180")
};

