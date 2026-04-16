// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Damageable.h"
#include "WeaknessActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FWeaknessBreakSignature,
	AWeaknessActor, FWeaknessBreakDelegate

);


UCLASS()
class WHITEBOX_API AWeaknessActor : public AActor,public IDamageable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaknessActor();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* DefaultRoot;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName BoneName;
	FWeaknessBreakSignature FWeaknessBreakDelegate;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UMaterialInterface> OnSelectMaterial;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UMaterialInterface> OnDeselectMaterial;
	void WeaknessSelect();
	void WeaknessDeselect();
	float WeaknessHP{ 50.f };
	virtual void TakeDamage(AActor* Causer,float Damage) override;
	UFUNCTION()
	void WeaknessBreakHandle();
};
