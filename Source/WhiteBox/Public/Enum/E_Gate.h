// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

 
UENUM(BlueprintType)
enum E_Gate{
	Walking UMETA(DisplayName = "Walking"),
	Jogging UMETA(DisplayName = "Jogging"),

};


UENUM(BlueprintType)
enum E_RootYawOffsetMode {
	BlendOut UMETA(DisplayName = "BlendOut"),
	Accumulate UMETA(DisplayName = "Accumulate"),
	Hold UMETA(DisplayName = "Hold"),	

};