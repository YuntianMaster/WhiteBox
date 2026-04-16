// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapon/GeneralProjectile.h"
#include "KnifeGeneralProjectile.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API AKnifeGeneralProjectile : public AGeneralProjectile
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bIsRandomPath{true};
	virtual void Fire(float CharingTime) override;
	AActor* ProjectileTarget;
	FVector RandomTargetLocation(AActor* Target);
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RandomTargetParam")
	float ProjectileSpeed{ 200.f };
	bool bIsRandomPathMove{ false };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RandomTargetParam")
	FVector SpawnRandomloc;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RandomTargetParam")
	float minDistance = 0.5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RandomTargetParam")
	float maxDistance = 0.7;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RandomTargetParam")
	float minOffset = 20;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RandomTargetParam")
	float maxOffset = 50;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RandomTargetParam")
	TSubclassOf<AActor> DebugActor;
	void SetTarget(AActor* Target);
	void StartRandomPath();
	bool ChecktheWeaknessHitHandle();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CheckRadiu{ 25.f };
	FHitResult OutHit;

};
