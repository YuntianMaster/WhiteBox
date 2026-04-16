// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Abilities/GameplayAbility.h"
#include "CamerManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHITEBOX_API UCamerManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCamerManagerComponent();

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AimFOV{ 60.f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector AimOffset{ 100.f,75.f,45.f };

	float initialFOV;
	FVector initialOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CameraMoveSpeed{ 100.f };

	UFUNCTION(BlueprintCallable)
	void CancleAimingCameraHandle();

	UFUNCTION(BlueprintCallable)
	void AimingCameraHandle();

protected:
	UFUNCTION()
	void Camera_NormalToAim();
	FTimerHandle FAimToNormalHandle;
	class  UCameraComponent* CameraRef;
	UFUNCTION()
	void Camera_AimToNormal();
	FTimerHandle FNormalToAimHandle;
	class USpringArmComponent* CamearBoom;
	void AimingTagChange(const FGameplayTag Callbacktage, int32 NewCount);

	virtual void BeginPlay() override;


		
};
