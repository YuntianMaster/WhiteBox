// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAS_BasicProps.generated.h"

UCLASS()
class WHITEBOX_API AGAS_BasicProps : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGAS_BasicProps();
	class UBasicAttributeSet* BasicAttributeSet;
	class UCombatAttributeSet* CombatAttributeSet;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
