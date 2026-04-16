// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/LookAtPlayerComp.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
ULookAtPlayerComp::ULookAtPlayerComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULookAtPlayerComp::BeginPlay()
{
	Super::BeginPlay();

	EnemyPawnRef = GetOwner();
	PlayerPawnRef = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	// ...
	
}


// Called every frame
void ULookAtPlayerComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector PlayerLocation{PlayerPawnRef->GetActorLocation() };
	FVector EnemyLocation{EnemyPawnRef->GetActorLocation()};
	FRotator DesiredRotator{ UKismetMathLibrary::FindLookAtRotation(EnemyLocation, PlayerLocation) };
	FRotator CurrentRotator{ EnemyPawnRef->GetActorRotation()};


	if (!bCanLookAt) return;
	FRotator NewRotator {
		UKismetMathLibrary::RInterpTo_Constant(
		CurrentRotator,
		DesiredRotator,
		DeltaTime,
		speed)
	};
	FRotator EnemyRotator{ CurrentRotator.Pitch,NewRotator.Yaw,CurrentRotator.Roll };


	EnemyPawnRef->SetActorRotation(EnemyRotator);

	// ...
}

