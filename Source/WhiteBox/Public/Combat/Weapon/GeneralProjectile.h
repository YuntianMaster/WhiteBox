// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Fighter.h"
#include "GeneralProjectile.generated.h"

class APickUpActor;
class UGameplayAbility;
UCLASS()
class WHITEBOX_API AGeneralProjectile : public AActor,public IFighter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGeneralProjectile();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UProjectileMovementComponent* ProjectileMoveComp;
	ACharacter* Char;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UAbilitySystemComponent* AbilitySysComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBasicAttributeSet* BasicAttributeSet;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UCombatAttributeSet* CombatAttributeSet;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<UGameplayAbility>> InitalAbilities;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadWrite)
	AActor* TargetActor;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void DestoryProjectile();
	void AttachProjectileToSocket();
	void DetachProjectileToSocket();

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float ProjectileMinSpeed{ 1000.f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ProjectileMaxSpeed{ 3000.f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxGravity{ 2.5f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MinGravity{ 0.5f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UNiagaraComponent* NiagaraComp;
	virtual void Fire(float CharingTime) ;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<APickUpActor> PickUpActor;
	UFUNCTION(BlueprintCallable)
	void OnHitHandle(AActor* HitObject);
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WeaponDamage{20.f};
	virtual float GetDamage() override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ProjectilePower{ 10000.f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsDebugMode{ true };
};
