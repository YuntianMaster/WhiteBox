// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Enemy.h"
#include "Player/StatsComponent.h"
#include "Enemy/EBossStats.h"
#include "Interface/Fighter.h"
#include "BossCharater.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnEnemyDeathSignature,
	ABossCharater, OnEnemyDeathDelegate
);

UCLASS()
class WHITEBOX_API ABossCharater : public ACharacter,public IEnemy,public IFighter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossCharater();
	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStatsComponent* statsComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCombatComponent* CombatComp;

	UFUNCTION(BlueprintCallable)
	void DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect);

	virtual float GetDamage() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EBossStats> InitalStats;

	class UBlackboardComponent* BlackboardComp;

	UFUNCTION(BlueprintCallable)
	void OnCombat();
	UFUNCTION(BlueprintCallable)
	virtual void Attack() override;

	UFUNCTION(BlueprintCallable)
	virtual float GetAttackDuration() override;

	virtual float GetMeleeRange() override;

	UFUNCTION()
	void HandleGameOver();

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UAnimMontage* DeathAnim;
	UPROPERTY(BlueprintAssignable)
	FOnEnemyDeathSignature OnEnemyDeathDelegate;
	UFUNCTION()
	virtual float OnHandleDeath() override;
	void FinishDeathAnim();
	virtual bool CanTakeDamage(AActor* Opponent) override;
};
