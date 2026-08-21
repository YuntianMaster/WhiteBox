// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy/WeaknessActor.h"
#include "LockComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnUpdatedTargetSignature,
	ULockComponent, OnUpdateTargetDelegate,
	AActor*, NewTargetActorRef
);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHITEBOX_API ULockComponent : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	ULockComponent();
	UPROPERTY(BlueprintAssignable)
	FOnUpdatedTargetSignature OnUpdateTargetDelegate;
	class UCamerManagerComponent* CamerManagerComp;
	class APlayerCharacter* OwnRef;
	APlayerController* Controller;
	class UCharacterMovementComponent* MovementComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Radiu = 750.0f;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnlockStart();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BreakDistance{ 1000.0f };
	

	UFUNCTION(BlueprintCallable)
	void ToggleLockOn();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY()
	AActor* CurrentTargetActor;
	UFUNCTION(BlueprintCallable)
	void UnlockTarget();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* BattleAction;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 WeaknessTargetNum{ 2 };
	TArray<AWeaknessActor*> TargetWeaknessActors;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float maxPitch{ -30.f };
};
