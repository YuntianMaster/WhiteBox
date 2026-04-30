// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_GameAblilityBase.h"
#include "Player/PlayerCharacter.h"
#include "Widget/SkillWidget.h"
#include "Widget/PlayerMainWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "AbilitySystemComponent.h"

UGA_GameAblilityBase::UGA_GameAblilityBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CharRef = nullptr;
	AnimInstance = nullptr;


}



void UGA_GameAblilityBase::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	TryCacheCharacterRefs(ActorInfo);
	UE_LOG(LogTemp, Warning, TEXT("Skill %s Get!!!"), *GetName());
	
	if (CharRef && CharRef->PlayerHUD)
	{
		CharRef->PlayerHUD->UpdatePlayerHUD();
	}

	if (bAutoActive)
	{
		UAbilitySystemComponent* ASCForActivate = nullptr;
		ASCForActivate = ActorInfo->AbilitySystemComponent.Get();
		
		if (ASCForActivate)
		{
			ASCForActivate->TryActivateAbility(Spec.Handle);
		}
	}


}

void UGA_GameAblilityBase::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	TryCacheCharacterRefs(ActorInfo);

}


void UGA_GameAblilityBase::TryCacheCharacterRefs(const FGameplayAbilityActorInfo* ActorInfo)
{
	CharRef = nullptr;
	AnimInstance = nullptr;

	if (ActorInfo)
	{
		CharRef = Cast<APlayerCharacter>(ActorInfo->AvatarActor.Get());
		if (!CharRef)
		{
			CharRef = Cast<APlayerCharacter>(ActorInfo->OwnerActor.Get());
		}
	}
	if (!CharRef)
	{
		CharRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	}
	if (!CharRef)
	{
		CharRef = Cast<APlayerCharacter>(GetOwningActorFromActorInfo());
	}

	if (!CharRef)
	{
		return;
	}

	

	AnimInstance = CharRef->GetMesh() ? CharRef->GetMesh()->GetAnimInstance() : nullptr;
}


