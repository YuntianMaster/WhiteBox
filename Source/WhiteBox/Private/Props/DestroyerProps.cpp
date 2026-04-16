 // Fill out your copyright notice in the Description page of Project Settings.


#include "Props/DestroyerProps.h"

// Sets default values
ADestroyerProps::ADestroyerProps()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));
	StaicMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaicMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADestroyerProps::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ADestroyerProps::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADestroyerProps::willBeDestroy()
{
	StaicMesh->SetOverlayMaterial(WarningMaterial);
}

