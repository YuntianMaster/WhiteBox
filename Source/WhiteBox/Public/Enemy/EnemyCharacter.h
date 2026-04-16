// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy/TakeDownComponent.h"
#include "Steal.h"
#include "Interface/Fighter.h"
#include "Interface/Enemy.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class WHITEBOX_API AEnemyCharacter : public ACharacter, public ISteal,public IFighter,public IEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTakeDownComponent* TakeDownComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UWeaknessSystem* WeaknessComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void beTakeDownHandle(UAnimMontage* beTakeDownAnim) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USceneActorWidget* UIComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UStatsComponent* StatsComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* CatchAnimMontage;
	virtual void Catch() override;

	USkeletalMeshComponent* SkelealComp;
	UAnimInstance* AnimInstance;
	FOnMontageEnded EndDelegate;

	UFUNCTION()
	void OnCatchHandle();
	bool bIsCatchComplete = false;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TSubclassOf<UUserWidget> CatchUI;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundBase* ShoutSound;
};
 