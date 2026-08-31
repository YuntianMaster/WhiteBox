// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CamerManagerComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Core/CameraVariableTable.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"


// Sets default values for this component's properties
UCamerManagerComponent::UCamerManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCamerManagerComponent::SetInitialCameraPose()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	UGameplayCameraComponent* CGC = Owner->FindComponentByClass<UGameplayCameraComponent>();
	if (!CGC)
	{
		return;
	}


	APlayerController* PC = Cast<APlayerController>(Owner->GetInstigatorController());
	if (!PC)
	{
		PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	}
	if (!PC || !PC->PlayerCameraManager)
	{
		return;
	}

	// 仍在 Lock Rig 上时读取当前镜头旋转，供 Third Person Entry 的 Context Yaw Pitch 使用。
	const FRotator CameraRotation = PC->PlayerCameraManager->GetCameraRotation();

	FBlueprintCameraPose InitialPose = CGC->GetInitialPose();
	InitialPose.Rotation = CameraRotation;
	CGC->SetInitialPose(InitialPose);

	UE_LOG(LogTemp, Warning, TEXT("SetInitialCameraPose: Yaw=%f Pitch=%f"), CameraRotation.Yaw, CameraRotation.Pitch);

	Table = CGC->GetInitialVariableTable();
}


void UCamerManagerComponent::CameraChangeHandle(UPARAM(meta = (Categories = "Camera")) FGameplayTag CameraTag)
{
	if(CurrentCameraTag == CameraTag )
		return;

	CurrentCameraTag = CameraTag;
	FOnCameraChangeDelegate.Broadcast(CameraTag);
			
}


void UCamerManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}



