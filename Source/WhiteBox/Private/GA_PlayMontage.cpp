// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_PlayMontage.h"
#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void UGA_PlayMontage::PlayMontageTask()
{
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		"MontagePlay",
		AnimMontage,
		PlayRate,
		NAME_None,
		true,
		1,
		0,
		true
	
	);

	MontageTask->OnBlendOut.AddDynamic(this,&UGA_PlayMontage::OnMontageEndHandle);
	MontageTask->OnCompleted.AddDynamic(this, &UGA_PlayMontage::OnMontageEndHandle);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_PlayMontage::OnMontageEndHandle);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_PlayMontage::OnMontageEndHandle);

	MontageTask->ReadyForActivation();
}

void UGA_PlayMontage::OnMontageEndHandle()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UGA_PlayMontage::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	AActor* PlayerActor = GetAvatarActorFromActorInfo();
	Cast<ACharacter>(PlayerActor)->GetMesh()->GetAnimInstance()->Montage_Stop(0.1f,AnimMontage);
		
}
