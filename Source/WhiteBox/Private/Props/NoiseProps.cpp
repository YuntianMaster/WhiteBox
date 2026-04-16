// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/NoiseProps.h"

// Sets default values
ANoiseProps::ANoiseProps()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANoiseProps::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANoiseProps::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

