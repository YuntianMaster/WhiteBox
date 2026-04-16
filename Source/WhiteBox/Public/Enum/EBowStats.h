// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EBowStats
{
	BowIdle UMETA(DisplayName = "BowIdle"),
	BowAim UMETA(DisplayName = "BowAim"),
	BowShoot UMETA(DisplayName = "BowShoot"),
	BowDraw UMETA(DisplayName = "BowDraw")


};
