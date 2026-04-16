// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
UENUM(BlueprintType)
enum EPlayerStates
{
	CharacterNoneStats UMETA(DisplayName = "CharacterNoneStats"),
	SwordDFence UMETA(DisplayName = "SwordDFence"),
	Aiming UMETA(DisplayName = "Aiming"),
	Drawing UMETA(DisplayName = "Drawing"),
	Rolling UMETA(DisplayName = "Rolling"),


};
