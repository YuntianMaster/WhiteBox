// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/PickUpActor.h"
#include "Player/PlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Travel/CollectionComp.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
APickUpActor::APickUpActor()
{
 

}

// Called when the game starts or when spawned
void APickUpActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void APickUpActor::InteractHandle()
{
	UE_LOG(LogTemp, Warning, TEXT("pickup!!"));
}



