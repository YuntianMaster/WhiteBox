// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/GAS_BasicProps.h"
#include "AttrubuteSet/BasicAttributeSet.h"
#include "AttrubuteSet/CombatAttributeSet.h"

// Sets default values
AGAS_BasicProps::AGAS_BasicProps()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BasicAttributeSet = CreateDefaultSubobject<UBasicAttributeSet>(TEXT("BasicAttributeSet"));
	CombatAttributeSet = CreateDefaultSubobject<UCombatAttributeSet>(TEXT("CombatAttributeSet"));

}

// Called when the game starts or when spawned
void AGAS_BasicProps::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGAS_BasicProps::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

