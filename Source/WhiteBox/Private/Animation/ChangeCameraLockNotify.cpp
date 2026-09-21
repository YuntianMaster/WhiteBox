// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ChangeCameraLockNotify.h"
#include "Player/CamerManagerComponent.h"
#include "Player/PlayerCharacter.h"
#include "Enemy/EnemyAIController.h"

void UChangeCameraLockNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp || !MeshComp->GetWorld() || !MeshComp->GetWorld()->IsGameWorld())
	{
		return;
	}
	AActor* Owner = MeshComp->GetOwner();
	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(Cast<APawn>(Owner)->GetController());
	if (!EnemyController) {
		UE_LOG(LogTemp, Warning, TEXT("EnemyAIController not found on actor: %s"), *Owner->GetName());
		return;
	}
	AActor* EnemyTargetActor = EnemyController->EnemyTargetActor;
	if(!EnemyTargetActor) {
		UE_LOG(LogTemp, Warning, TEXT("EnemyTargetActor not found on EnemyAIController: %s"), *EnemyController->GetName());
		return;
	}
	UCamerManagerComponent* CameraManagerComp = EnemyTargetActor->FindComponentByClass<UCamerManagerComponent>();
	if (!CameraManagerComp) {

		UE_LOG(LogTemp, Warning, TEXT("CameraManagerComponent not found on actor: %s"), *EnemyTargetActor->GetName());
		return;
	}
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(EnemyTargetActor);
	AActor* PlayerTargetActor = PlayerCharacter->TargetActor;
	if(PlayerTargetActor == Owner)
		CameraManagerComp->CameraModeChangeHandle(CameraLockMode);
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter's TargetActor is not the owner of this notify: %s"), *Owner->GetName());
		if(PlayerTargetActor)
			UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter's TargetActor is : %s"), *PlayerTargetActor->GetName());
	}
}