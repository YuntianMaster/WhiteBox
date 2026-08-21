// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapon/GeneralProjectile.h"
#include "GeneralProjectile_AI.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API AGeneralProjectile_AI : public AGeneralProjectile
{
	GENERATED_BODY()
public:
	virtual void Fire(float CharingTime) override;
	virtual void Tick(float DeltaTime) override;

protected:
	void SetBezierVelocity();

protected:
	AActor* OwnerActor;
	AActor* EnemyTargetActor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectileSettings")
	float ProjectileSpeed = 1000.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BezierSettings")
	float MaxOffset = 1.5f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BezierSettings")
	float MinOffset = 0.5f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BezierSettings")
	float CurveDuration = 0.3f;
	FVector BezierLocation;
	float ProjectileMoveTime = 0.f;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BezierSettings")
	bool bIsRight = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BezierSettings")
	bool bIsUp = true;

	
};
