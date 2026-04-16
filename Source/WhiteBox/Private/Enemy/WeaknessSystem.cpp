// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/WeaknessSystem.h"
#include "GameFramework/Character.h"
#include "Player/PlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Structure/FWeaknessStruct.h"
#include "Algo/RandomShuffle.h"
#include "Enemy/WeaknessActor.h"

// Sets default values for this component's properties
UWeaknessSystem::UWeaknessSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaknessSystem::BeginPlay()
{
	Super::BeginPlay();
	CreateWeakness();
	// ...
	
}


// Called every frame
void UWeaknessSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaknessSystem::OnSelect()
{
	for (AActor* Weakness : AWeaknessActors) {

		//UE_LOG(LogTemp, Warning, TEXT("box name %s"), *Weakness->GetName());
	
		Weakness->GetComponentByClass<UStaticMeshComponent>()->SetHiddenInGame(false);
	}

	RandomWeaknessTargetSelect();
}

void UWeaknessSystem::OnDeselect()
{
	for (AActor* Weakness : AWeaknessActors) {

		Weakness->GetComponentByClass<UStaticMeshComponent>()->SetHiddenInGame(true);
		SelectedAWeaknessActors.Empty();
	}
}

void UWeaknessSystem::CreateWeakness() {

	USkeletalMeshComponent* Mesh =  GetOwner<ACharacter>()->GetMesh();
	FAttachmentTransformRules AttachmentRule(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true);


	if (WeaknessActorStructs.Num() == 0) return;
	for (const FWeaknessStruct& WeaknessActorStruct : WeaknessActorStructs) {
		FTransform Socketransfrom = Mesh->GetSocketTransform(WeaknessActorStruct.SocketName, RTS_World);
		AWeaknessActor* WeaknessPoint = GetWorld()->SpawnActor<AWeaknessActor>(WeaknessActorStruct.WeaknessActor, Socketransfrom);
		WeaknessPoint->GetRootComponent()->AttachToComponent(Mesh, AttachmentRule, WeaknessActorStruct.SocketName);
		WeaknessPoint->WeaknessHP = WeaknessActorStruct.HP;
		AWeaknessActors.Add(WeaknessPoint);
	}
	RandomWeaknessTargetSelect();
}


void UWeaknessSystem::RandomWeaknessTargetSelect()	
{
	APlayerCharacter* PlayerRef = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!PlayerRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("RandomWeaknessTarget : No PlayerRef"));
		return;
	}
	int32 num = PlayerRef->LockComp->WeaknessTargetNum;


	for (AWeaknessActor* WeaknessPoint : AWeaknessActors) {

		WeaknessPoint->WeaknessDeselect();
	
	}


	if (AWeaknessActors.Num() <= num)
	{
		for (AWeaknessActor* WeaknessPoint : AWeaknessActors) {

			WeaknessPoint->WeaknessSelect();
			SelectedAWeaknessActors.Add(WeaknessPoint);
		}

	}

	else {
		
		TArray<AWeaknessActor*> Shuffled = AWeaknessActors;
		Algo::RandomShuffle(Shuffled);
		for (size_t i = 0; i < num; i++)
		{
			Shuffled[i]->WeaknessSelect();
			SelectedAWeaknessActors.Add(Shuffled[i]);
		}
	}
}

AActor* UWeaknessSystem::GetWeaknessTarget()
{
	
	int i = UKismetMathLibrary::RandomIntegerInRange(0, SelectedAWeaknessActors.Num());
	return 	SelectedAWeaknessActors[i];
	;
}

