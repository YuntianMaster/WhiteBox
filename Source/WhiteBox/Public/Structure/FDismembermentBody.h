// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FDismembermentBody.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct WHITEBOX_API FDismembermentBody
{
	GENERATED_BODY();
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USkeletalMesh* Mesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName BoneName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BodyHP;

};
