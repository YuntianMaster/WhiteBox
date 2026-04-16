// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/WeaknessActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"

// Sets default values
AWeaknessActor::AWeaknessActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(DefaultRoot);
	DefaultRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("DefaultRoot"));
	StaticMesh->SetupAttachment(DefaultRoot);
}

// Called when the game starts or when spawned
void AWeaknessActor::BeginPlay()
{
	Super::BeginPlay();
	FWeaknessBreakDelegate.AddDynamic(this, &AWeaknessActor::WeaknessBreakHandle);


}

// Called every frame
void AWeaknessActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaknessActor::WeaknessSelect()
{
	if (OnSelectMaterial)
	{
		//UE_LOG(LogTemp, Warning, TEXT("SelectWeakness"));
		const int32 ApplySlots = FMath::Max(1, StaticMesh ? StaticMesh->GetNumMaterials() : 0);
		for (int32 i = 0; i < ApplySlots; i++)
		{
			StaticMesh->SetMaterial(i, OnSelectMaterial);
		}
	}
}

void AWeaknessActor::WeaknessDeselect()
{
	if (OnDeselectMaterial)
	{
		const int32 ApplySlots = FMath::Max(1, StaticMesh ? StaticMesh->GetNumMaterials() : 0);
		for (int32 i = 0; i < ApplySlots; i++)
		{
			StaticMesh->SetMaterial(i, OnDeselectMaterial);
		}
	}
}

void AWeaknessActor::TakeDamage(AActor* Causer,float Damage)
{
	if (WeaknessHP <= 0) return;
	float currentHP = WeaknessHP - Damage;
	WeaknessHP = UKismetMathLibrary::FClamp(currentHP, 0, 50);
	UE_LOG(LogTemp, Warning, TEXT("HP: %f"), WeaknessHP);
	if (WeaknessHP<=0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Br1eak!"));
		FWeaknessBreakDelegate.Broadcast();
	}
}

void AWeaknessActor::WeaknessBreakHandle()
{

	UE_LOG(LogTemp, Warning, TEXT("Break!"));
}

