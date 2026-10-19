// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCameraArmStruct.generated.h"

USTRUCT(BlueprintType)
struct WHITEBOX_API FCameraArmStruct
{
	GENERATED_BODY();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float MinBoomLength = 800.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float MaxBoomLength = 2500.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float MinBoomHeight = 100.f;      // BoomOffset.Z
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float MaxBoomHeight = 200.f;      // BoomOffset.Z
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float MinBoomLatera = 80.f;      // BoomOffset.Y
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockTarget")
	float MaxBoomLatera = 200.f;      // BoomOffset.Z

};


