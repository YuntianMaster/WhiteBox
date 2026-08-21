// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_RollAbility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionWarpingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GameFramework/Character.h"

void UGA_RollAbility::GA_Roll()
{
	UE_LOG(LogTemp, Warning, TEXT("Rolling"));
	CharacterRef = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	FVector RollDirection{
		
		CharacterRef->GetCharacterMovement()->Velocity.Length() < 1.f ?
		CharacterRef->GetActorForwardVector() :
		CharacterRef->GetLastMovementInputVector().GetSafeNormal()
	};
	const float ActorYaw = CharacterRef->GetActorForwardVector().Rotation().Yaw;
	const float InputYaw = RollDirection.GetSafeNormal2D().Rotation().Yaw;

	const float DeltaSigned = FMath::FindDeltaAngleDegrees(ActorYaw, InputYaw); // -180~180
	const float Angle360 = FRotator::ClampAxis(DeltaSigned);                     // 0~360
	/*UE_LOG(LogTemp, Warning, TEXT("last input yaw,%f"), Angle360);*/

	
	
	
	//UE_LOG(LogTemp, Warning, TEXT("yaw::%f"), RotatYaw);
	int CounterMax = Roll_F.Num();
	RollIndex = UKismetMathLibrary::Wrap(RollIndex, -1, CounterMax - 1);
	/*UE_LOG(LogTemp, Warning, TEXT("last input,%s"), *RollDirection.ToString());
	UE_LOG(LogTemp, Warning, TEXT("last input length,%f"), RollDirection.Length());*/
	float Duration = 0;
	UAbilityTask_PlayMontageAndWait* RollMontage = nullptr;

	//  不同角度播放不同翻滚动画
	if (Angle360 < 30)
	{
		if (EROLLDir != ERollDirection::F)
		{
			RollIndex = 0;
			CharacterRef->GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
		}
		EROLLDir = ERollDirection::F;
		Duration = Roll_F[RollIndex]->GetPlayLength();
		UE_LOG(LogTemp, Warning, TEXT("During,%f"), Duration);
		RollMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"Rolling",
			Roll_F[RollIndex],
			1,
			NAME_None,
			true,
			1,
			0,
			false);



		
	}

	else if(Angle360 < 60)
	{
		if (EROLLDir != ERollDirection::FR)
		{
			CharacterRef->GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
			RollIndex = 0;
			CharacterRef->GetMesh()->SetRelativeRotation(FRotator(0, -90 + Angle360, 0));
		}
		EROLLDir = ERollDirection::FR;
		
		Duration = Roll_R[RollIndex]->GetPlayLength();
		
		RollMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"Rolling",
			Roll_F[RollIndex],
			1,
			NAME_None,
			true,
			1,
			0,
			false
		);
		
	}


	else if (Angle360 < 120)
	{
		Duration = Roll_R[RollIndex]->GetPlayLength();
		UE_LOG(LogTemp, Warning, TEXT("R,%f"), Duration);
		if (EROLLDir != ERollDirection::R)
		{
			RollIndex = 0;
			CharacterRef->GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
		}
		EROLLDir = ERollDirection::R;
		
		
		RollMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"Rolling",
			Roll_R[RollIndex],
			1,
			NAME_None,
			true,
			1,
			0,
			false
		);
		
	}


	else if (Angle360 < 150)
	{
		if (EROLLDir != ERollDirection::BR)
		{
			RollIndex = 0;
			CharacterRef->GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
			CharacterRef->GetMesh()->SetRelativeRotation(FRotator(0, -90 + Angle360 + 180, 0));
		}
		EROLLDir = ERollDirection::BR;
		UE_LOG(LogTemp, Warning, TEXT("During,%f"), Duration);
		Duration = Roll_B[RollIndex]->GetPlayLength();

	
		
		
		RollMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"Rolling",
			Roll_B[RollIndex],
			1,
			NAME_None,
			true,
			1,
			0,
			false
		);

		

	}

	else if (Angle360 < 210) {
		if (EROLLDir != ERollDirection::B)
		{
			RollIndex = 0;
			CharacterRef->GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
		}
		EROLLDir = ERollDirection::B;
		UE_LOG(LogTemp, Warning, TEXT("During,%f"), Duration);
		Duration = Roll_R[RollIndex]->GetPlayLength();
		RollMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"Rolling",
			Roll_B[RollIndex],
			1,
			NAME_None,
			true,
			1,
			0,
			false
		);

	}

	else if (Angle360 < 240)
	{
		if (EROLLDir != ERollDirection::BL)
		{
			RollIndex = 0;
			CharacterRef->GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
			CharacterRef->GetMesh()->SetRelativeRotation(FRotator(0, -90 + Angle360 + 180, 0));
		}
		EROLLDir = ERollDirection::BL;
		UE_LOG(LogTemp, Warning, TEXT("During,%f"), Duration);
		Duration = Roll_B[RollIndex]->GetPlayLength();
		RollMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"Rolling",
			Roll_B[RollIndex],
			1,
			NAME_None,
			true,
			1,
			0,
			false
		);
	

	}
	else if (Angle360 < 300)
	{
		if (EROLLDir != ERollDirection::L)
		{
			CharacterRef->GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
			RollIndex = 0;
		}
		EROLLDir = ERollDirection::L;
		UE_LOG(LogTemp, Warning, TEXT("During,%f"), Duration);
		Duration = Roll_L[RollIndex]->GetPlayLength();
		RollMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"Rolling",
			Roll_L[RollIndex],
			1,
			NAME_None,
			true,
			1,
			0,
			false
		);
	
	}
	else if(Angle360 < 330){
		if (EROLLDir != ERollDirection::FL)
		{
			CharacterRef->GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
			RollIndex = 0;
			CharacterRef->GetMesh()->SetRelativeRotation(FRotator(0, -90 + Angle360, 0));
		}
		EROLLDir = ERollDirection::FL;
		UE_LOG(LogTemp, Warning, TEXT("During,%f"), Duration);
		Duration = Roll_F[RollIndex]->GetPlayLength();
		RollMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"Rolling",
			Roll_F[RollIndex],
			1,
			NAME_None,
			true,
			1,
			0,
			false
		);

	}
	
	UE_LOG(LogTemp, Warning, TEXT("EROLLDir, %d"), static_cast<int>(EROLLDir));
	if (RollMontage)
	{
		RollMontage->OnCompleted.AddDynamic(this, &UGA_RollAbility::ResetMeshDirection);
		RollMontage->ReadyForActivation();
	/*	RollMontage->OnBlendOut.AddDynamic(this, &UGA_RollAbility::ResetMeshDirection);
		RollMontage->OnCancelled.AddDynamic(this, &UGA_RollAbility::ResetMeshDirection);
		RollMontage->OnCompleted.AddDynamic(this, &UGA_RollAbility::ResetMeshDirection);*/
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Roll montage task was not created."));
	}


	UAbilityTask_WaitDelay* DelayRollActive = UAbilityTask_WaitDelay::WaitDelay(this, Duration*0.6);
	DelayRollActive->OnFinish.AddDynamic(this,&UGA_RollAbility::GA_RollActive);
	DelayRollActive->ReadyForActivation();
	RollIndex++;
	

	CharacterRef->GetWorldTimerManager().ClearTimer(RollFinishTimeHandler);
	CharacterRef->GetWorldTimerManager().SetTimer(
		RollFinishTimeHandler,
		this,
		&UGA_RollAbility::GA_RollEnd,
		Duration + 0.5f,
		false
	);

}

void UGA_RollAbility::GA_RollActive()
{
	UE_LOG(LogTemp, Warning, TEXT("Roll montage reset."));
	//BP_RemoveGameplayEffectFromOwnerWithHandle(RollingHandle, -1);

}

void UGA_RollAbility::GA_RollEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("Roll End."));
	RollIndex = 0;
	UE_LOG(LogTemp, Warning, TEXT("Roll End tag: %s"), *BlockAbilitiesWithTag.First().ToString());
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UGA_RollAbility::ResetMeshDirection()
{
	UE_LOG(LogTemp, Warning, TEXT("Roll Reset Mesh."));
	CharacterRef->GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	EROLLDir = ERollDirection::NOINPUT;

}
