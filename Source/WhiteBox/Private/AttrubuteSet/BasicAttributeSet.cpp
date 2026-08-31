// Fill out your copyright notice in the Description page of Project Settings.


#include "AttrubuteSet/BasicAttributeSet.h"

#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Perception/AISense_Damage.h"
#include "Enemy/EnemyAIController.h"
#include "Player/PlayerCharacter.h"
#include "Enemy/EnemyCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/StateTreeAIComponent.h"
#include "Net/UnrealNetwork.h"


UBasicAttributeSet::UBasicAttributeSet()
{
	Health = 100.f;
	MaxHealth = 100.f;
	Stamina = 100.f;
	MaxStamina = 100.f;
	Damage = 0.f;
	Shield = 0.f;
	MaxShield = 100.f;
	Poise = 100.f;
	MaxPoise = 100.f;
}

void UBasicAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

void UBasicAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{

	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
		UE_LOG(LogTemp, Warning, TEXT("Pre health:%f"), GetHealth());
	}
	if (Attribute == GetStaminaAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0, GetMaxStamina());
		
	}
	if (Attribute == GetShieldAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0, GetMaxShield());
	}

	if (Attribute == GetPoiseAttribute()) {

		NewValue = FMath::Clamp(NewValue, 0, GetMaxPoise());
	}


}


void UBasicAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	UE_LOG(LogTemp, Warning, TEXT("Effect executed"));
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const TSharedPtr<FGameplayAbilityActorInfo> TargetActorInfo = Data.Target.AbilityActorInfo;
		AActor* TargetActor = TargetActorInfo.IsValid() ? TargetActorInfo->AvatarActor.Get() : nullptr;
		AActor* CauserActor = Data.EffectSpec.GetEffectContext().GetInstigator();

		if (TargetActor && TargetActor->GetWorld())
		{
			const FVector HitLoc = TargetActor->GetActorLocation();
			const FVector EventLoc = CauserActor ? CauserActor->GetActorLocation() : HitLoc;
			UAISense_Damage::ReportDamageEvent(
				TargetActor->GetWorld(),
				TargetActor,
				CauserActor,
				GetDamage(),
				EventLoc,
				HitLoc,
				FName(TEXT("Attribute Damage Event Report"))
			);
		}

		float totalDamage = GetDamage();
		SetDamage(0.f);

		if (GetShield() > 0)
		{
			float RemainingDamage = totalDamage - GetShield();
			SetShield(GetShield() - totalDamage);
			if (RemainingDamage > 0)
			{
				SetHealth(GetHealth() - RemainingDamage);
			}
		}
		else
		{
			SetHealth(GetHealth() - totalDamage);
		}

		if (GetMaxHealth() > 0.f && GetHealth() / GetMaxHealth() < 0.5f)
		{
			const FGameplayAbilityActorInfo* ActorInfo = GetActorInfo();
			AActor* AvatarActor = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr;
			AEnemyAIController* EnemyAIController = AvatarActor
				? Cast<AEnemyAIController>(AvatarActor->GetInstigatorController())
				: nullptr;
			if (EnemyAIController)
			{
				EnemyAIController->OnHP75Delegate.Broadcast();
			}
		}
	} 


	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(GetHealth());

	}

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute()) {
		SetStamina(GetStamina());
	}

	if(Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		
		SetMaxHealth(GetMaxHealth());
	}

	if (Data.EvaluatedData.Attribute == GetPoiseAttribute()) {

		SetPoise(GetPoise());

		if (GetPoise() == GetMaxPoise()) {
			UE_LOG(LogTemp, Warning, TEXT("PostGameplayEffectExecute: Poise MAX"));
			OnPoiseGetMaxHandle(Data);
			Poise = 0;
			SetPoise(0);

			UE_LOG(LogTemp, Warning, TEXT("After zero: GetPoise=%f Base=%f Current=%f"),
				GetPoise(), Poise.GetBaseValue(), Poise.GetCurrentValue());
			if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
			{
				UE_LOG(LogTemp, Warning, TEXT("ASC Numeric: %f"),
					ASC->GetNumericAttribute(GetPoiseAttribute()));
			}
		}


	}

}

void UBasicAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	if(Attribute == GetMaxHealthAttribute())
	{
		SetHealth(NewValue);
	}
}

//Posie�������ִ���߼�
void UBasicAttributeSet::OnPoiseGetMaxHandle(const FGameplayEffectModCallbackData& Data)
{
	//�����߼�ִ��
	AActor* OwnerActor = GetOwningActor();
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerActor->GetInstigatorController());
	if (EnemyAIController)
	{
		UStateTreeAIComponent* StateTreeAIComp = EnemyAIController->GetComponentByClass<UStateTreeAIComponent>();
		FGameplayTag PoiseMaxTage = FGameplayTag::RequestGameplayTag("Event.StateTree.PoiseUptoMax");
		StateTreeAIComp->SendStateTreeEvent(PoiseMaxTage);

		const FGameplayEffectContextHandle& Context = Data.EffectSpec.GetEffectContext();

		UE_LOG(LogTemp, Warning, TEXT("Poise GE Instigator :%s"), *Context.GetInstigator()->GetName());

	}

	//����߼�ִ��
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OwnerActor);
	if (PlayerCharacter)
	{
		UAbilitySystemComponent* ASC = PlayerCharacter->AbilitySystemComp;
		FGameplayTag PoiseMaxTage = FGameplayTag::RequestGameplayTag("Event.PoiseUptoMax");
		FGameplayEventData Payload;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			PlayerCharacter,
			PoiseMaxTage,
			Payload

		);

	}

}
