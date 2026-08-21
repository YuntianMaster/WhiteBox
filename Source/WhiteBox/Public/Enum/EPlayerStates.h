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
	Parrying UMETA(DisplayName = "Parrying"),
	PoiseMax UMETA(DisplayName = "PoiseMax")

};


UENUM(BlueprintType)

enum EPlayerVelocityDirection
{
	VelocityNone UMETA(DisplayName = "VelocityNone"),
	VelocityForward UMETA(DisplayName = "VelocityForward"),
	VelocityBackward UMETA(DisplayName = "VelocityBackward"),
	VelocityLeft UMETA(DisplayName = "VelocityLeft"),
	VelocityRight UMETA(DisplayName = "VelocityRight"),
	VelocityFR UMETA(DisplayName = "VelocityFR"),
	VelocityFL UMETA(DisplayName = "VelocityFL"),
	VelocityBR UMETA(DisplayName = "VelocityBR"),
	VelocityBL UMETA(DisplayName = "VelocityBL"),
};

UENUM(BlueprintType)
enum EAnimState {
		AnimStateNone UMETA(DisplayName = "AnimStateNone"),
		AnimStart UMETA(DisplayName ="AnimStart"),
		AnimCycle UMETA(DisplayName = "AnimCycle"),
		AnimEnd UMETA(DisplayName = "AnimEnd"),
};
