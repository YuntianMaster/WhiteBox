// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum EEnemyStats {
	NoneStats UMETA(DisplayName = "NoneStats"),
	Patrol UMETA(DisplayName = "Patrol"),
	Chase UMETA(DisplayName = "Chase"),
	Alarm UMETA(DisplayName = "Alarm")
};
