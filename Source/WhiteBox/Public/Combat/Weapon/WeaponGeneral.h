// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enum/EWeapon.h"
#include "Structure/FWeaponStruct.h"
#include "Enemy/WeaknessActor.h"
#include "NiagaraDataInterfaceExport.h"
#include "WeaponGeneral.generated.h"



UCLASS()
class WHITEBOX_API AWeaponGeneral : public AActor, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AWeaponGeneral();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	ACharacter* PlayerRef;
	
	

public:	
	UPROPERTY(BlueprintReadWrite)
	USkeletalMeshComponent* WeaponMeshComp;
	UPROPERTY(BlueprintReadWrite)
	class UNiagaraComponent* WeaponNiagaraComp;
	FName NiragaraAttachSocketName{ TEXT("NiragaraSocket") };
	UFUNCTION(BlueprintCallable)
	void InitWeapon(FWeaponStruct WeaponStruct);
	FWeaponStruct WeaponStructData;
	UFUNCTION(BlueprintCallable)
	void RegisterDataInterface();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Niagara")
	void ReceiveParticleData(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset);
	virtual void ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset) override;

private:
	void SetupNiagaraFromStruct(const FWeaponStruct& WeaponStruct);
};
