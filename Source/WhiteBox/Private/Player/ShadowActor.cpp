// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ShadowActor.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"

// Sets default values
AShadowActor::AShadowActor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShadowActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShadowActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShadowActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AShadowActor::RunBT(UAnimMontage* AnimRef , FTransform TargetRef)
{
	TakeDownMontage = AnimRef;
	TargetTrans = TargetRef;
	AAIController* AICon = Cast<AAIController>(GetController());
	if (AICon)
		AICon->RunBehaviorTree(BT_Shadow);

}

