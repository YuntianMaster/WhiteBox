// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DebugLibrary.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UDebugLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Debug")
	static FString GetScriptStackTrace();
	
};
