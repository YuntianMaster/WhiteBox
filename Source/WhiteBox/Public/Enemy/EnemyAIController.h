// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enum/ECombatEnum.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"


DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnHP75Signature, 
	AEnemyAIController, OnHP75Delegate
);

/**
 * 
 */
UCLASS()
class WHITEBOX_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:

	AEnemyAIController();
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void isSeeingPlayer(const FAIStimulus& Stimulus);
	UFUNCTION()
	void OnPerceptionUpdatedHandle(AActor* Actor, FAIStimulus Stimulus);

	//UFUNCTION(BlueprintCallable)
	//void InitialEnemyBlackBoard();

	
	FOnHP75Signature OnHP75Delegate;
protected:
	FTimerHandle LostSennStimulusHandle;
	UFUNCTION()
	void LostSeenStimulusHandler();


public:



	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UAIPerceptionComponent* AIPerceptionComp;
	UPROPERTY(BlueprintReadWrite)
	AActor* EnemyTargetActor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UStateTreeAIComponent* StateTreeAIComp;
	//UFUNCTION()
	//void OnHP75Handle();
	//void SetEnemyCombatSpeedsByFloat(float CustomCombatSpeed);
	//void SetEnemyCombatSpeedsByEnum(ECombatSpeeds CombatSpeedEnum);
};
