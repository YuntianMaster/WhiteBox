// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy/WeaknessActor.h"
#include "Structure/FWeaknessStruct.h"
#include "WeaknessSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHITEBOX_API UWeaknessSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaknessSystem();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<FName> WeaknessActors;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AWeaknessActor> WeaknessActor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FWeaknessStruct> WeaknessActorStructs;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void OnSelect();
	UFUNCTION(BlueprintCallable)
	void OnDeselect();
	void RandomWeaknessTargetSelect();
	AActor* GetWeaknessTarget();
	void CreateWeakness();
	TArray<AWeaknessActor*> AWeaknessActors;
	TArray<AWeaknessActor*> SelectedAWeaknessActors;
		
};
