// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UnlockActionComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UUnlockActionComponent::UUnlockActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUnlockActionComponent::BeginPlay()
{
	Super::BeginPlay();
	if (APawn* Pawn = Cast<APawn>(GetOwner()))
	{
		if (APlayerController* PC = Cast<APlayerController>(Pawn->GetController()))
		{
			if (ULocalPlayer* LP = PC->GetLocalPlayer())
			{
				InputSystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			}
		}
	}

}


// Called every frame
void UUnlockActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UUnlockActionComponent::LockInputMovement(FName reason)
{
	if (!LockReason.Contains(reason))
	{
		LockReason.Add(reason);
		ApplyLock();
	}
}

void UUnlockActionComponent::UnlockInputMovement(FName reason)
{
	if (LockReason.Contains(reason))
	{
		LockReason.Remove(reason);
		
	}

	if (LockReason.Num() == 0)
		ApplyUnlock();
}

void UUnlockActionComponent::ApplyLock()
{
	InputSystem->ClearAllMappings();
}

void UUnlockActionComponent::ApplyUnlock()
{
	InputSystem->AddMappingContext(DefaultContentMapping,0);
}

