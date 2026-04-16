// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/LockComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interface/Enemy.h"
#include "Interface/MainPlayerInterface.h"
#include "Player/PlayerCharacter.h"
#include "Enemy/WeaknessActor.h"
#include "Enemy/WeaknessSystem.h"
#include "GameFramework/SpringArmComponent.h"



// Sets default values for this component's properties
ULockComponent::ULockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULockComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnRef = GetOwner<ACharacter>();
	Controller = GetWorld()->GetFirstPlayerController();
	MovementComp = OwnRef->GetCharacterMovement();
	SpringArmComp = OwnRef->FindComponentByClass<USpringArmComponent>();

}

void ULockComponent::OnlockStart(float Radiu)
{
	 
	FHitResult Outresult;

	
	FVector CurrentLocation{ GetOwner()->GetActorLocation() };
	FCollisionShape Sphere{ FCollisionShape::MakeSphere(Radiu) };
	FCollisionQueryParams IngoreParams{
		FName{"Ingore Params"},false,GetOwner()
	};

	bool TargetLocked{
		GetWorld()->SweepSingleByChannel(Outresult,
			CurrentLocation,
			CurrentLocation,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel1,
			Sphere,
			IngoreParams
			)
	};
	UE_LOG(LogTemp, Warning, TEXT("LockingStart!"));
	if (!TargetLocked) return;
	UE_LOG(LogTemp, Warning, TEXT("LockingTargetFind!"));
	// check enemy
	if (!Outresult.GetActor()->Implements<UEnemy>()) return;
	UE_LOG(LogTemp, Warning, TEXT("LockingUEnemy!"));
	CurrentTargetActor =  Outresult.GetActor();

	IEnemy::Execute_OnSelect(CurrentTargetActor);
	//WeaknessActors Update;
	UWeaknessSystem* EnemyWeakSys = CurrentTargetActor->GetComponentByClass<UWeaknessSystem>();
	if (!EnemyWeakSys) {

		UE_LOG(LogTemp, Warning, TEXT("Enemy NoWeakSystem"));
		TargetWeaknessActors.Empty();
	}

	else
	{

		TargetWeaknessActors = EnemyWeakSys->SelectedAWeaknessActors;
		UE_LOG(LogTemp, Warning, TEXT("TargetWeaknessActors num: %d"), TargetWeaknessActors.Num());
		
	}

	
	OnUpdateTargetDelegate.Broadcast(CurrentTargetActor);
	Controller->SetIgnoreLookInput(true);
	MovementComp->bOrientRotationToMovement = false;
	MovementComp->bUseControllerDesiredRotation = true;
	
	//Change to BattleMode
	float During = OwnRef->PlayAnimMontage(BattleAction);
	APlayerCharacter* CharacterRef{ GetOwner<APlayerCharacter>() };
	if (CharacterRef->Implements<UMainPlayerInterface>())
		IMainPlayerInterface::Execute_DisablePlayerInput(CharacterRef);
	FTimerHandle StopMoveHandler;
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([CharacterRef](){
		if (CharacterRef)
			IMainPlayerInterface::Execute_EnablePlayerInput(CharacterRef);
	});
	GetOwner()->GetWorldTimerManager().SetTimer(
		StopMoveHandler,
		TimerDel,
		During,
		false
	);

	
}

void ULockComponent::UnlockTarget()
{
	TargetWeaknessActors.Empty();
	if (!IsValid(CurrentTargetActor)) return;

	IEnemy::Execute_OnDeSelect(CurrentTargetActor); 
	CurrentTargetActor = nullptr;
	SpringArmComp->TargetOffset = FVector{ 0.0f, 0.0f, 0.0f };
	Controller->SetIgnoreLookInput(false);
	MovementComp->bOrientRotationToMovement = true;
	MovementComp->bUseControllerDesiredRotation = false;
	OnUpdateTargetDelegate.Broadcast(CurrentTargetActor);

}

void ULockComponent::ToggleLockOn(float Radius)
{

	if (IsValid(CurrentTargetActor))
		UnlockTarget();
	else OnlockStart(Radius);


}



// Called every frame
void ULockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector CurrentLocation{ OwnRef->GetActorLocation() };
	if (!IsValid(CurrentTargetActor)) return;
	FVector TargetLocation{ CurrentTargetActor->GetActorLocation() };
	TargetLocation.Z -= 100.0f;
	FRotator newRotation{ UKismetMathLibrary::FindLookAtRotation(CurrentLocation,TargetLocation) };
	double TargetDistance{ FVector::Distance(CurrentLocation,TargetLocation) };
	if (TargetDistance > BreakDistance)
	{
		UnlockTarget();
		return;
	}
	newRotation.Pitch = UKismetMathLibrary::FClamp(newRotation.Pitch, maxPitch, newRotation.Pitch);
	Controller->SetControlRotation(newRotation);
	//UE_LOG(LogTemp, Warning, TEXT("TargetWeaknessActors num: %s"), *newRotation.ToString());
	SpringArmComp->TargetOffset = FVector{0.0f, 0.0f, 100.0f};
	// ... 
}

