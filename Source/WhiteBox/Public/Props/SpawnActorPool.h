// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Props/SpawnActor.h"
#include "SpawnActorPool.generated.h"

UCLASS()
class WHITEBOX_API ASpawnActorPool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnActorPool();

	struct PoolActor{

		ASpawnActor* SpawnActor;
		bool bIsVaild;
	};

	TArray<PoolActor> ActorsPool;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int PoolSize{ 5 };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int SpawnTimes{ 3 };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AActor*> SpawnLocations;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* EndLocation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Pool")
	TSubclassOf<ASpawnActor> SpawnActorClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SpawnFrequency{ 1.0f };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	PoolActor* FindFirstValidActor();
	UFUNCTION()
	void SpawnActorHandle();
	FTimerHandle FSpawnHandle;
	void DestoryPoolActor(PoolActor* poolActor);

};
