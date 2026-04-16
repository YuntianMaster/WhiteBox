// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_ComboAbility.h"
#include "Combat/LockComponent.h"
#include "GameFramework/Character.h"
#include "HAL/PlatformTime.h"
#include "Kismet/KismetMathLibrary.h"
#include "MotionWarpingComponent.h"

UGA_ComboAbility::UGA_ComboAbility()
{
	OnGameplayAbilityCancelled.AddUObject(this, &UGA_ComboAbility::OnCancelHandle);
	
}

void UGA_ComboAbility::OnCancelHandle()
{
	CombotCounter = 0;
	BP_RemoveGameplayEffectFromOwnerWithHandle(AttackingHandle, -1);
	Cast<ACharacter>(GetAvatarActorFromActorInfo())->StopAnimMontage(CurrentMontage);
}

void UGA_ComboAbility::GA_ComboAttackHandle()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackingStart11"));
	
	if (ComboMontages.Num() == 0)
		return;
	
	UE_LOG(LogTemp, Warning, TEXT("AttackingStart"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Stats.Attacking"));
	AttackingSpecHandle = MakeOutgoingGameplayEffectSpec(GE_Attacking, 1.0f);
	AttackingHandle = K2_ApplyGameplayEffectSpecToOwner(AttackingSpecHandle);

	////Montage Play
	float AttackPlayDuration = Cast<ACharacter>(GetAvatarActorFromActorInfo())->PlayAnimMontage(ComboMontages[CombotCounter]);
	CurrentMontage = ComboMontages[CombotCounter];
	float InputTiming = 0.0f;
	float InputPassTiming = 0.0f;
	CombotCounter++;

	//WarpMotion
	ULockComponent* LockComp = GetAvatarActorFromActorInfo()->GetComponentByClass<ULockComponent>();
	if (LockComp->CurrentTargetActor)
	{
		FVector start = GetAvatarActorFromActorInfo()->GetActorLocation();
		FVector Target = LockComp->CurrentTargetActor->GetActorLocation();
		FVector Distance = Target - start;
		Distance = Distance.GetSafeNormal();
		FVector WarpTarget = Target - Distance * TargetOffset;
		float dis = FVector::Distance(start, Target);
		UMotionWarpingComponent* MotionWarpComp = GetAvatarActorFromActorInfo()->GetComponentByClass<UMotionWarpingComponent>();
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
	int CounterMax{ ComboMontages.Num() };
	CombotCounter = UKismetMathLibrary::Wrap(CombotCounter, -1, CounterMax - 1);
	////Stop input



	GetWorld()->GetTimerManager().ClearTimer(GA_FCombotInputHandle);
	GetWorld()->GetTimerManager().ClearTimer(GA_FCombotInputPassHandle);
	GetWorld()->GetTimerManager().SetTimer(
		GA_FCombotInputHandle,
		this,
		&UGA_ComboAbility::GA_FCombotInput,
		InputTiming,
		false
	);
	GetWorld()->GetTimerManager().SetTimer(
		GA_FCombotInputPassHandle,
		this,
		&UGA_ComboAbility::GA_FCombotInputPass,
		InputPassTiming,
		false
	);
	
}

void UGA_ComboAbility::GA_FCombotInput()
{
	
	UE_LOG(LogTemp, Warning, TEXT("Input True"));
	BP_RemoveGameplayEffectFromOwnerWithHandle(AttackingHandle,-1);
}

void UGA_ComboAbility::GA_FCombotInputPass()
{
	UE_LOG(LogTemp, Warning, TEXT("EndAblity"));
	CombotCounter = 0;
	BP_RemoveGameplayEffectFromOwnerWithHandle(AttackingHandle, -1);
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}


