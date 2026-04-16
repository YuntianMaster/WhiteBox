// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/PlayerBlockComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/MainPlayerInterface.h"

// Sets default values for this component's properties
UPlayerBlockComponent::UPlayerBlockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerBlockComponent::BeginPlay()
{
	Super::BeginPlay();
	CharacterRef = GetOwner<ACharacter>();
	MoveComp = CharacterRef->GetCharacterMovement();
	// ...
	
}


// Called every frame
void UPlayerBlockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UPlayerBlockComponent::Check(AActor* Opponent)
{
	
	FVector PlayerForward = GetOwner()->GetActorForwardVector();
	FVector EnemyForward = Opponent->GetActorForwardVector();


	if (!CharacterRef->Implements<UMainPlayerInterface>()) return true;
	IMainPlayerInterface* PlayerRef{ GetOwner<IMainPlayerInterface>() };
	double Result{ FVector::DotProduct(PlayerForward, EnemyForward) };
	
	if (Result > 0 || !PlayerRef->HasEnoughStamina(BlockingStaminaCost))
		return true;

	float Duration{ CharacterRef->PlayAnimMontage(BlockAnim)};
	OnBlockCostDelegate.Broadcast(BlockingStaminaCost);

	MoveComp->DisableMovement();                    // ½ûÖ¹ÒÆ¶¯
	
	 


	FTimerHandle BlockingDuration;

	CharacterRef->GetWorldTimerManager().SetTimer(
		BlockingDuration,
		this,
		&UPlayerBlockComponent::RegenMovement,
		Duration,
		false
	);

	return false;
}



void UPlayerBlockComponent::RegenMovement()
{
	
	MoveComp->SetMovementMode(MOVE_Walking);                    // ½ûÖ¹ÒÆ¶¯
	
}


