// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact.h"
#include "Props/InteractProps.h"
#include "PickUpActor.generated.h"

UCLASS()
class WHITEBOX_API APickUpActor : public AInteractProps
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void InteractHandle() override;
};
