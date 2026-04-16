// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "ShadowActor.generated.h"

UCLASS()
class WHITEBOX_API AShadowActor : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShadowActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UBehaviorTree* BT_Shadow;

	void RunBT(UAnimMontage* AnimRef ,FTransform TargetRef);
	UPROPERTY(BlueprintReadWrite)
	UAnimMontage* TakeDownMontage;
	UPROPERTY(BlueprintReadWrite)
	FTransform TargetTrans;

};
