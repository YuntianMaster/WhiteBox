 // Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_BowAimingAbility.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Abilities/GameplayAbilityTargetActor_SingleLineTrace.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Camera/CameraComponent.h"



void UGA_BowAimingAbility::AimingStart()
{

	//UE_LOG(LogTemp, Warning, TEXT("CancelAimingHandle"));
	if (!AimingMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("AimingMontage is null!"));
		return;
	}

	if (!CreatedArrow)
	{
		CreatedArrow = GetWorld()->SpawnActor(Arrow, &GetAvatarActorFromActorInfo()->GetActorTransform());

		if (!CharRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("AimingStart:: NoCharRef"));
			return;
		}
		USkeletalMeshComponent* Mesh = CharRef->GetMesh();
		if (!Mesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("Mesh is null!"));
			return;
		}
		FAttachmentTransformRules ArrowAttachRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			true
		);
		CreatedArrow->GetRootComponent()->AttachToComponent(Mesh, ArrowAttachRules, "arrow");


	}

	CreatedArrow->SetActorHiddenInGame(true);

	CharRef->PlayAnimMontage(AimingMontage);
	CharRef->GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed;

	UAbilityTask_WaitGameplayEvent* WaitCancelAimingEvent =
		UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("Event.Combat.CancelAiming"));
	WaitCancelAimingEvent->EventReceived.AddDynamic(this, &UGA_BowAimingAbility::CancelAimingHandle);
	WaitCancelAimingEvent->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitGetArrowEvent =
		UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("Event.Combat.GetArrow"));
	WaitGetArrowEvent->EventReceived.AddDynamic(this, &UGA_BowAimingAbility::GetArrow);
	WaitGetArrowEvent->ReadyForActivation();

	// Register all listeners before broadcasting aiming event to avoid missing early notify events.
	FGameplayEventData Payload;
	SendGameplayEvent(FGameplayTag::RequestGameplayTag("Event.Combat.Aiming"), Payload);
}

void UGA_BowAimingAbility::CancelAimingHandle(FGameplayEventData Payload)
{
	(void)Payload;
	UE_LOG(LogTemp, Warning, TEXT("CancelAimingHandle"));
	CreatedArrow->SetActorHiddenInGame(true); 
	if (!CharRef || !CharRef->GetMesh())
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		return;
	}
	AnimInstance->Montage_Stop(0.1f, AimingMontage);
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UGA_BowAimingAbility::ShootingHandle(FGameplayEventData Payload)
{

	UE_LOG(LogTemp, Warning, TEXT("ShootingHandle"));
	(void)Payload;
	

}

void UGA_BowAimingAbility::GetArrow(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("GetArrow:: GetArrow"));
	
	CreatedArrow->SetActorHiddenInGame(false);
}


void UGA_BowAimingAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UE_LOG(LogTemp, Warning, TEXT("UGA_BowAimingAbility:: End"));
	CreatedArrow->SetActorHiddenInGame(true);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}









