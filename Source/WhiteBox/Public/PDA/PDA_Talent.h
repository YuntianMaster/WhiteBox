// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PDA_Talent.generated.h"


DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FTalentLevelUpDelegate, UPDA_Talent, OnTalentGet);
/**
 * 
 */
UCLASS()
class WHITEBOX_API UPDA_Talent : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FString TalentName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UTexture2D* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TArray<TSubclassOf<class UGameplayEffect>> GEs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TArray<TSubclassOf<class UGameplayAbility>> GAs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	int32 CurrentLevel = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	int32 MaxLevel = 4;
	FTalentLevelUpDelegate OnTalentGet;
};
