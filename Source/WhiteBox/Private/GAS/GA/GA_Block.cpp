// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_Block.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

void UGA_Block::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	/*UAbilityTask_WaitGameplayEvent * BlockHitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag("Event.Combat.Block"));
	BlockHitGameplayEvent->EventReceived.AddDynamic(this, &UGA_Block::BlockHandler);*/
}

void UGA_Block::BlockHandler(FGameplayEventData Payload)
{
	NoWarpMontageHandle();
}
