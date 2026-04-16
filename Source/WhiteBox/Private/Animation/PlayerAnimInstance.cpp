// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PlayerAnimInstance.h"
#include "Combat/WeaponSystemComp.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"

void UPlayerAnimInstance::UpdateTheVelocity()
{

	APawn* Apawnref{ TryGetPawnOwner() };
	if (!IsValid(Apawnref)) return;
	FVector velocity{ Apawnref->GetVelocity() };
	CurrentVelocity = static_cast<float>(velocity.Length());
	//CurrentVelocity = velocity.Length();
	FRotator AimDirect = Apawnref->GetBaseAimRotation();
	FRotator ActorDirect = Apawnref->GetActorRotation();
	FRotator AimingRo = (AimDirect - ActorDirect).GetNormalized();
	AimingPitch = AimingRo.Pitch;
}

void UPlayerAnimInstance::HandleUpdateTarget(AActor* NewTargetActorRef)
{
	bIsIncombat = IsValid(NewTargetActorRef);

}

void UPlayerAnimInstance::UpdateDirection()
{
	APawn* Apawnref{ TryGetPawnOwner() };
	if (!Apawnref) return;
	//if (!bIsIncombat) return;

	CurrentDirect = CalculateDirection(Apawnref->GetVelocity(), Apawnref->GetActorRotation());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{

	Super::NativeUpdateAnimation(DeltaTimeX);
	APawn* Apawnref{ TryGetPawnOwner() };
	APlayerCharacter* CharRef = Cast<APlayerCharacter>(Apawnref);
	if (!CharRef) return;
	ECurrentWeapon = CharRef->GetComponentByClass<UWeaponSystemComp>()->ECurrentWeapons;

}

//void UPlayerAnimInstance::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
//
//	UE_LOG(LogTemp, Warning, TEXT("Ended!!!"));
//
//}
