// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ProjectileInteractable.h"
#include "Interface/Damageable.h"
#include "Blueprint/UserWidget.h"
#include "Interface/Fighter.h"
#include "ProjectileTargetActor.generated.h"

UCLASS()
class WHITEBOX_API AProjectileTargetActor : public AActor, public IProjectileInteractable, public IDamageable,public IFighter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileTargetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void TargetAddImpulse(FVector ImpulseVector, float Power, FName BoneName) override;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	USkeletalMeshComponent* SkeletlMeshComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UWidgetComponent* UserWidgetComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UStatsComponent* StatsComp;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="UI")
	TSubclassOf<UDamagePropsWidget> TargetWidgetClass;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	float existTime{ 1.0f };
	virtual void TakeDamage(AActor* Causer,float Damage) override;
	// 缓存 WidgetComponent 创建出的具体 Widget 实例
	UPROPERTY(Transient)
	UDamagePropsWidget* DamagePropsObject = nullptr;
	virtual float OnHandleDeath() override;

	UFUNCTION()
	void HideUI();


};
