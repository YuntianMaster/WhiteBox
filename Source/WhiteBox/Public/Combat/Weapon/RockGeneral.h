// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapon/WeaponGeneral.h"
#include "Structure/FProjecTilePoolActor.h"
#include "Combat/Weapon/GeneralProjectile.h"
#include "RockGeneral.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API ARockGeneral : public AWeaponGeneral
{
	GENERATED_BODY()

public:
	ARockGeneral();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TSubclassOf<AGeneralProjectile> RockProjectile;
	virtual void BeginPlay() override;
	virtual void BlockFuction() override;
	virtual void ResertWeapon() override;
	virtual void ActiveWeapon() override;
	virtual void Shooting()override;
	virtual void ReleaseCharing() override;
	void DrawTraceLine();

	AActor* CreatedRockProjectile;
	class USplineComponent* SplineComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float PowerUpMax = 6;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PowerUpMin = 2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PowerOffset = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Power = 1000;

	TArray<TObjectPtr<AActor>> ActorsToIgnore;
	FTimerHandle DrawTraceHandle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DrawFrequency{ 0.5f };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectileTrace")
	UStaticMesh* TraceMesh;
	// 可选：不填则使用 TraceMesh 资源自带的材质
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectileTrace")
	TObjectPtr<class UMaterialInterface> TraceMaterial;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectileTrace")
	int PoolSize = 20;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectileTrace")
	float TraceLifeTime = 2.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectileTrace")
	float LastFrequcy = 0.05f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectileTrace")
	TArray<FProjecTilePoolActor> TracePool;
	FTimerHandle PoolLiftTimeHandle;
	FProjecTilePoolActor* GetFirstUnActivedActor();
	void addSplineMesh();
	void ResetProjectileHandle(FProjecTilePoolActor* ProjectActor);
};
