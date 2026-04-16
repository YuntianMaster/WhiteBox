// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/LeverSwitch.h"
#include "Animation/AnimInstance.h"
#include "Components/SphereComponent.h"

// Sets default values
ALeverSwitch::ALeverSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(FName("MeshComp"));
	MeshComp->AttachToComponent(Cast<USceneComponent>(Super::SphereActiveArea), FAttachmentTransformRules::SnapToTargetIncludingScale);
}

// Called when the game starts or when spawned
void ALeverSwitch::BeginPlay()
{
	Super::BeginPlay();
	if(SenorRef)
		DeactivateActor(SenorRef);
}

// Called every frame
void ALeverSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALeverSwitch::InteractHandle()
{
	if (SenorRef && !bIsActived)
	{
		UE_LOG(LogTemp, Warning, TEXT("on! "));
		ActivateActor(SenorRef);
		bIsActived = true;
		MeshComp->PlayAnimation(AnimSwitchOn, false);
	
	}


	else if (SenorRef && bIsActived)
	{
		UE_LOG(LogTemp, Warning, TEXT("off! "));
		DeactivateActor(SenorRef);
		bIsActived = false;
		MeshComp->PlayAnimation(AnimSwitchOff, false);
	}
}

void ALeverSwitch::DeactivateActor(ASensor* Actor)
{
	Actor->SetActorHiddenInGame(true);
	Actor->SetActorTickEnabled(false);
	Actor->SetActorEnableCollision(false);
}

void ALeverSwitch::ActivateActor(ASensor* Actor)
{
	Actor->SetActorHiddenInGame(false);
	Actor->SetActorTickEnabled(true);
	Actor->SetActorEnableCollision(true);
}



