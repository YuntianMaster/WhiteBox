// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatComponent.h"
#include "Player/PlayerCharacter.h"
#include "Combat/WeaponSystemComp.h"
#include "Combat/Weapon/WeaponGeneral.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interface/MainPlayerInterface.h"
#include "Player/UnlockActionComponent.h"
#include "MotionWarpingComponent.h"
#include "Animation/PlayerAnimInstance.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}




// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterRef = GetOwner<APlayerCharacter>();
	WeanponSystemRef = GetOwner()->GetComponentByClass<UWeaponSystemComp>();
	LockActionRef = GetOwner()->GetComponentByClass<UUnlockActionComponent>();
	AnimInstance = Cast<UPlayerAnimInstance>(GetOwner<ACharacter>()->GetMesh()->GetAnimInstance());
	// ...
	
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

void UCombatComponent::UpdateResetAttack()
{
	bFirstCanAttack = true;
}

void UCombatComponent::RandomAttack()
{
	int AttackIndex{ UKismetMathLibrary::RandomInteger(AttackAnimations.Num())};

	//UE_LOG(LogTemp, Warning, TEXT("%i - %i"), AttackAnimations.Num(), AttackIndex);
	AttackDurationTime = CharacterRef->PlayAnimMontage(AttackAnimations[AttackIndex]);

}

void UCombatComponent::OnAttackPerformed()
{
	OnAttackPerformDelegate.Broadcast(StaminaCost);
}


void UCombatComponent::ComboAttackHandle()
{
	
	if (WeanponSystemRef->ECurrentWeapons == EWeapons::NoWeapon)
	{	
		return;
	}

	if (!bCanComboInput) return;
	bCanComboInput = false;
	////Combo Group
	int ComboGroupIndex = WeanponSystemRef->CurrentWeapon->ComboGroupIndex;
	TArray<FComboAttackMontages> ComboAttackMontages =  WeanponSystemRef->CurrentWeapon->ComboAttackMontages;
	if (ComboAttackMontages.Num() == 0)
		return;
	////Montage Play
	float AttackPlayDuration = WeanponSystemRef->CurrentWeapon->WeaponAttack(ComboAttackMontages[ComboGroupIndex].CombAttackMontages[CombotCounter]);
	float InputTiming = 0.0f;
	float InputPassTiming = 0.0f;

	//WarpMotion
	ULockComponent* LockComp = CharacterRef->GetComponentByClass<ULockComponent>();
	if (LockComp->CurrentTargetActor)
	{
		FVector start = GetOwner()->GetActorLocation();
		FVector Target = LockComp->CurrentTargetActor->GetActorLocation();
		FVector Distance = Target - start;
		Distance = Distance.GetSafeNormal();
		FVector WarpTarget = Target - Distance * TargetOffset;
		float dis = FVector::Distance(start, Target);
		UMotionWarpingComponent* MotionWarpComp = GetOwner()->GetComponentByClass<UMotionWarpingComponent>();
		if (dis < MotionWarpDistance)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), dis);
			MotionWarpComp->AddOrUpdateWarpTargetFromLocation("Attack", WarpTarget);
		}

		else
		{
			MotionWarpComp->RemoveWarpTarget("Attack");

		}
	}



	//InputTimeing
	InputTiming = AttackPlayDuration * (1 - attackTimingPercent);
	//LockActionRef->LockInputMovement("ComboAttack");
	InputPassTiming = AttackPlayDuration + attackBufferTime;
	CombotCounter++;
	int CounterMax{ ComboAttackMontages[ComboGroupIndex].CombAttackMontages.Num()};
	CombotCounter = UKismetMathLibrary::Wrap(CombotCounter, -1, CounterMax - 1);
	////Stop input
	
	
	//combo input timing
	GetWorld()->GetTimerManager().ClearTimer(FCombotInputHandle);
	GetWorld()->GetTimerManager().ClearTimer(FCombotInputPassHandle);
	GetWorld()->GetTimerManager().SetTimer(
		FCombotInputHandle,
		this,
		&UCombatComponent::ComboInputHandle,
		InputTiming,
		false
	);
	GetWorld()->GetTimerManager().SetTimer(
		FCombotInputPassHandle,
		this,
		&UCombatComponent::ComboInputPassHandle,
		InputPassTiming,
		false
	);
}

void UCombatComponent::ChangeToTravelMode()
{
	CharacterRef->PlayAnimMontage(ChangetoTravel);
}

void UCombatComponent::BlockAttackHandle()
{
	if (WeanponSystemRef->ECurrentWeapons == EWeapons::NoWeapon) return;
	WeanponSystemRef->CurrentWeapon->BlockFuction();
	//CharacterRef->PlayAnimMontage(WeanponSystemRef->CurrentWeapon->BlockMontage);
	//AnimInstance->bIsBowAiming = true;
}

void UCombatComponent::CancelBlockAttackHandle()
{
	if (WeanponSystemRef->ECurrentWeapons == EWeapons::NoWeapon) return;
	WeanponSystemRef->CurrentWeapon->CancelBlockFuction();
	//AnimInstance->bIsBowAiming = false;
}

void UCombatComponent::CombatThrow()
{
	if (WeanponSystemRef->ECurrentThrowWeapons == EWeapons::NoWeapon) return;

	TArray<FComboAttackMontages> ComboAttackMontages = WeanponSystemRef->CurrentThrowWeapon->ComboAttackMontages;
	WeanponSystemRef->CurrentThrowWeapon->WeaponAttack(ComboAttackMontages[0].CombAttackMontages[0]);
}

void UCombatComponent::DefenseAttackHandle()
{
	CharacterRef->PlayAnimMontage(WeanponSystemRef->CurrentWeapon->DenfenseAttack);
}

void UCombatComponent::RollingAttackHandle()
{
	CharacterRef->PlayAnimMontage(WeanponSystemRef->CurrentWeapon->RollingAttack, 1, "ROLLATTACK");
}

void UCombatComponent::AttackFunction()
{
	if (CharacterRef->PlayerStats == EPlayerStates::Rolling) {
		RollingAttackHandle();
		return;
	}

	if (CharacterRef->PlayerStats == EPlayerStates::SwordDFence) {
		DefenseAttackHandle();
		return;

	}

	ComboAttackHandle();
}

void UCombatComponent::ComboInputHandle()
{
	bCanComboInput = true;
	//LockActionRef->UnlockInputMovement("ComboAttack");
}

void UCombatComponent::ComboInputPassHandle()
{
	CombotCounter = 0;
	bCanComboInput = true;
}

