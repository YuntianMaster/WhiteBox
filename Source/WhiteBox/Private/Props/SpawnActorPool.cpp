// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/SpawnActorPool.h"
#include "Kismet/KismetMathLibrary.h"
#include "Props/SpawnActor.h"

// Sets default values
ASpawnActorPool::ASpawnActorPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
}

// Called when the game starts or when spawned
void ASpawnActorPool::BeginPlay()
{
	Super::BeginPlay();
	if (!SpawnActorClass)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnActorPool: SpawnActorClass is null."));
		return;
	}

	if (SpawnFrequency <= 0.f)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnActorPool: SpawnFrequency must be > 0. Current=%f"), SpawnFrequency);
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("SpawnActorPool: SpawnActorClass is begining."));
	for (size_t i = 0; i < PoolSize; i++)
	{
		ASpawnActor* newActor = GetWorld()->SpawnActor<ASpawnActor>(SpawnActorClass, GetActorTransform());
		if (!newActor)
		{
			UE_LOG(LogTemp, Error, TEXT("SpawnActorPool: SpawnActor failed."));
			continue;
		}
		newActor->SetActorHiddenInGame(true);
		newActor->StaticMeshcomp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		newActor->SkeletalMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		newActor->StaticMeshcomp->SetSimulatePhysics(false);
		newActor->SkeletalMeshComp->SetSimulatePhysics(false);
		ActorsPool.Add(PoolActor{ newActor ,true });
		
	}

	// 只启动一次定时器，避免在 Tick 里反复重置导致回调不触发。
	GetWorld()->GetTimerManager().SetTimer(
		FSpawnHandle,
		this,
		&ASpawnActorPool::SpawnActorHandle,
		SpawnFrequency,
		true
	);
}

// Called every frame
void ASpawnActorPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ASpawnActorPool::PoolActor* ASpawnActorPool::FindFirstValidActor()
{
	for (ASpawnActorPool::PoolActor& ActorRef : ActorsPool)
	{
		if (ActorRef.bIsVaild)
			return &ActorRef;
	}
	return nullptr;
}

void ASpawnActorPool::SpawnActorHandle()
{
	for (size_t i = 0; i < SpawnTimes; i++)
	{
		if (!FindFirstValidActor()) return;
		
		ASpawnActorPool::PoolActor* actorRef{ FindFirstValidActor() };
		actorRef->bIsVaild = false;
		actorRef->SpawnActor->SetActorHiddenInGame(false);
		actorRef->SpawnActor->StaticMeshcomp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		actorRef->SpawnActor->SkeletalMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		int32 index = UKismetMathLibrary::RandomInteger(SpawnLocations.Num());
		actorRef->SpawnActor->SetActorLocation(SpawnLocations[index]->GetActorLocation());
		actorRef->SpawnActor->StaticMeshcomp->SetSimulatePhysics(true);
		actorRef->SpawnActor->SkeletalMeshComp->SetSimulatePhysics(true);
	}
	
}

void ASpawnActorPool::DestoryPoolActor(PoolActor* poolActor)
{	poolActor->bIsVaild = true;
	poolActor->SpawnActor->SetActorHiddenInGame(true);
	poolActor->SpawnActor->StaticMeshcomp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	poolActor->SpawnActor->SkeletalMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	poolActor->SpawnActor->StaticMeshcomp->SetSimulatePhysics(false);
	poolActor->SpawnActor->SkeletalMeshComp->SetSimulatePhysics(false);}