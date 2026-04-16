// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structure/FRopeSockets.h"
#include "TravelingComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHITEBOX_API UTravelingComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTravelingComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	FTimerHandle TurnHandle;
	FTimerHandle MoveHandle;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void TryGetRopeStartLoc();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FRopeSockets RopeSocket;
	USkeletalMeshComponent* SkeletalComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DistanceCheck{ 200.f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	double CheckSize{ 40.f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bDebugMode{ false };
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UAnimMontage* RopeClimb;
	UPROPERTY(BlueprintReadWrite)
	bool bIsRopeClimbing{ false };
	APlayerController* PlayerControlRef;
	UFUNCTION()
	void TurnCameraBehind();
	UFUNCTION()
	void MovetoLocation(FVector Endlocation);
	UFUNCTION()
	void StartMove(FVector Endlocation,float Duration);
	UFUNCTION()
	void StartMoveHandle(FVector Endlocation);
	class UCharacterMovementComponent* Movementcomp;
	FHitResult OutHit;
	bool CheckStartPoint();

	class ARopeActor* RopeRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* LandingMontage;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* NormalLandingMontage;

	UFUNCTION(BlueprintCallable)
	void LandingHandle();
	FOnMontageEnded MontageEndDelegate;
	UFUNCTION()
	void MontageEndHandler();
	class UUnlockActionComponent* LockActionComp;
};
