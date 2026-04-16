// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/TakeDownComponent.h"
#include "Enemy/EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Widget/SceneActorWidget.h"
#include "Player/PlayerActionsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BrainComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"


// Sets default values for this component's properties
UTakeDownComponent::UTakeDownComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UTakeDownComponent::BeginPlay()
{
	Super::BeginPlay();
	

	ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(this, 0);

	if (PlayerChar)
	{
		PlayerActionRef = PlayerChar->FindComponentByClass<UPlayerActionsComponent>();	
	}
	
	
}


// Called every frame
void UTakeDownComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!PlayerActionRef) return;
	if (!(PlayerActionRef->TakeDownTarget)) 
		SetUIInvisable();
	if (GetOwner() == PlayerActionRef->TakeDownTarget)
		SetUIVisable();
		
	// ...
}

void UTakeDownComponent::UnderTakeDown(UAnimMontage* UnderTakeDown)
{
	ACharacter* CharRef = Cast<ACharacter>(GetOwner());
	CharRef->PlayAnimMontage(UnderTakeDown);
	GetOwner()->FindComponentByClass<UCapsuleComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (!CharRef) return;
	AAIController* ControllerRef{ CharRef->GetController<AAIController>() };
	if(!ControllerRef)return;
	ControllerRef->GetBrainComponent()->StopLogic("Defeat");
}

void UTakeDownComponent::SetUIVisable()
{
	Cast<AEnemyCharacter>(GetOwner())->UIComp->SetVisibility(true);
}

void UTakeDownComponent::SetUIInvisable()
{
	Cast<AEnemyCharacter>(GetOwner())->UIComp->SetVisibility(false);
}

