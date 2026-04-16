// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SceneActorWidget.h"

void USceneActorWidget::TurnToCamera()
{
	UWorld* World = GetWorld();
	if (!World) return; // 先检查 World
	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC) return; // 再检查 PlayerController

	FRotator CameraRot = PC->GetControlRotation();
	this->SetWorldRotation(CameraRot);
	FRotator CamraRot = GetWorld()->GetFirstPlayerController()->GetControlRotation();
	CamraRot.Pitch += 180.f;
	CamraRot.Roll += 180.f;
	CamraRot.Normalize();
	this->SetWorldRotation(CamraRot);
}

void USceneActorWidget::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TurnToCamera();
}
