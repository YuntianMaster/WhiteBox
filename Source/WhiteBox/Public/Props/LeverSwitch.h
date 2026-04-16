// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Props/InteractProps.h"
#include "Interact.h"
#include "Alarm/Sensor.h"
#include "LeverSwitch.generated.h"

UCLASS()
class WHITEBOX_API ALeverSwitch : public AInteractProps
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeverSwitch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void InteractHandle() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimationAsset* AnimSwitchOn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimationAsset* AnimSwitchOff;
	bool bIsCanInteract{false};
	UPROPERTY(BlueprintReadWrite)
	bool bIsActived {false};
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class ASensor* SenorRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USkeletalMeshComponent* MeshComp;

	void DeactivateActor(ASensor* Actor);
	void ActivateActor(ASensor* Actor);
};
