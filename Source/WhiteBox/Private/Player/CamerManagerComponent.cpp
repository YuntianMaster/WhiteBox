// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CamerManagerComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemComponent.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values for this component's properties
UCamerManagerComponent::UCamerManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}




void UCamerManagerComponent::CancleAimingCameraHandle()
{
	GetWorld()->GetTimerManager().ClearTimer(FAimToNormalHandle);
	GetWorld()->GetTimerManager().ClearTimer(FNormalToAimHandle);
	GetWorld()->GetTimerManager().SetTimer(
		FNormalToAimHandle,
		this,
		&UCamerManagerComponent::Camera_AimToNormal,
		GetWorld()->DeltaTimeSeconds,
		true
	);
}

void UCamerManagerComponent::AimingCameraHandle()
{
	UE_LOG(LogTemp, Warning, TEXT("AimingCameraHandle : Aiming!!"));
	GetWorld()->GetTimerManager().ClearTimer(FAimToNormalHandle);
	GetWorld()->GetTimerManager().ClearTimer(FNormalToAimHandle);
	GetWorld()->GetTimerManager().SetTimer(
		FAimToNormalHandle,
		this,
		&UCamerManagerComponent::Camera_NormalToAim,
		GetWorld()->DeltaTimeSeconds,
		true
	);

}

void UCamerManagerComponent::Camera_NormalToAim()
{
	if (FMath::IsNearlyEqual(CameraRef->FieldOfView, AimFOV, 5.f))
		GetWorld()->GetTimerManager().ClearTimer(FNormalToAimHandle);
	CameraRef->FieldOfView = UKismetMathLibrary::FInterpTo_Constant(CameraRef->FieldOfView, AimFOV, GetWorld()->DeltaTimeSeconds, CameraMoveSpeed);
	float percent = (CameraRef->FieldOfView - initialFOV) / (AimFOV - initialFOV);
	CamearBoom->SocketOffset = UKismetMathLibrary::VLerp(CamearBoom->SocketOffset, AimOffset, percent);
}

void UCamerManagerComponent::Camera_AimToNormal()
{

	if (FMath::IsNearlyEqual(CameraRef->FieldOfView, initialFOV, 5.f))
		GetWorld()->GetTimerManager().ClearTimer(FAimToNormalHandle);
	CameraRef->FieldOfView = UKismetMathLibrary::FInterpTo_Constant(CameraRef->FieldOfView, initialFOV, GetWorld()->DeltaTimeSeconds, CameraMoveSpeed);
	float percent = (CameraRef->FieldOfView - AimFOV) / (initialFOV - AimFOV);
	CamearBoom->SocketOffset = UKismetMathLibrary::VLerp(CamearBoom->SocketOffset, initialOffset, percent);
}

void UCamerManagerComponent::AimingTagChange(const FGameplayTag Callbacktage, int32 NewCount)
{
	if (NewCount > 0) {

		UE_LOG(LogTemp, Warning, TEXT("AimingTagChange : Aiming!!"));
		AimingCameraHandle();
	}
	else {

		CancleAimingCameraHandle();

	}
}

void UCamerManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	CameraRef = Cast<ACharacter>(GetOwner())->GetComponentByClass<UCameraComponent>();
	CamearBoom = Cast<ACharacter>(GetOwner())->GetComponentByClass<USpringArmComponent>();
	initialFOV = CameraRef->FieldOfView;

	UE_LOG(LogTemp, Warning, TEXT("initialFOV %f"), initialFOV);
	GetOwner<APlayerCharacter>()->AbilitySystemComp->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("Stats.Aiming")).
		AddUObject(this, &UCamerManagerComponent::AimingTagChange);
}



