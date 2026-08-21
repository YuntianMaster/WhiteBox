// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "FGASStucts.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct WHITEBOX_API FTraceMontageID: public FGameplayAbilityTargetData
{
	GENERATED_BODY()
	

	UPROPERTY(BlueprintReadWrite)
	int32 TRACE_MONTAGE_ID = INDEX_NONE;

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FTraceMontageID::StaticStruct();
	}
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << TRACE_MONTAGE_ID;
		bOutSuccess = true;
		return true;
	}

};
