// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Alarm/Sensor.h"
#include "AlarmSensor.generated.h"

UCLASS()
class WHITEBOX_API AAlarmSensor : public ASensor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAlarmSensor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void RotHead();
	void UpdateRotation();
	float LeftLimit{ -50.0f };
	float RightLimit{ 50.0f };
	float TargetRot{ 50.0f };
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float speed{ 10.0f };
	USceneComponent* HeadComp = nullptr;


	float GetYaw();
	void SetYaw(float Yaw);
	UFUNCTION(BlueprintCallable)
	void HandleStartAlarm(AActor* Player);

	class URotatingMovementComponent* RoatatingMoveComp;
};
