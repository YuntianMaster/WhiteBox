// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UCLASS()
class WHITEBOX_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ParticleTemple;
	class UProjectileMovementComponent* ParticleMovementComp;

	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float FProjectDamage{ 10.0f };
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void HandleBeginOverlap(AActor* otherActor);
	UFUNCTION()
	void DestoryProjectile();


};
