// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/StatsComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"
#include "Interface/Damageable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AttrubuteSet/BasicAttributeSet.h"
#include "Interface/Fighter.h"

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
	// ...
}

void UStatsComponent::ReduceHealth(float Damage, AActor* Opponent)
{
	if (Stats[EStats::Health] <= 0) return;

	
	IFighter* FighterRef{ GetOwner<IFighter>() };
	IDamageable* DamageableRef{ GetOwner<IDamageable>() };
	if (!DamageableRef) return;


	UE_LOG(LogTemp, Warning, TEXT("CAN DAMAGE!"));


	float currentHealth{ Stats[EStats::Health] };
	float maxHealth{ Stats[EStats::MaxHealth] };
	float updateHealth{ Stats[EStats::Health] - Damage };
	Stats[EStats::Health] = UKismetMathLibrary::FClamp(updateHealth,0, maxHealth);

	OnHealthPercentUpdateDelegate.Broadcast(GetStatPercetage(EStats::Health, EStats::MaxHealth));

	if (Stats[EStats::Health] == 0)
	{
		FighterRef = GetOwner<IFighter>();
		float DeathDuration = FighterRef->OnHandleDeath();
		UE_LOG(LogTemp, Warning, TEXT("STATS - START,%f"), DeathDuration);
		FLatentActionInfo FucntionInfo{
		0,
		101,
		TEXT("HandleOnDeath"),
		this
		};
		UKismetSystemLibrary::Delay(GetWorld(),DeathDuration, FucntionInfo);

	}


}

void UStatsComponent::ReduceStamina(float Amount)
{
	if (Stats[EStats::Stamina] <= 0) return;
	Stats[EStats::Stamina] -= Amount;
	Stats[EStats::Stamina] = UKismetMathLibrary::FClamp(Stats[EStats::Stamina], 0.0f, Stats[EStats::MaxStamina]);
	bCanRegenStamina = false;
	FLatentActionInfo FucntionInfo{
		0,
		100,
		TEXT("EnableRegen"),
		this
	};
	UKismetSystemLibrary::RetriggerableDelay(GetWorld(), StaminaDelayDuration, FucntionInfo);
	OnStaminaPercentUpdateDelegate.Broadcast(GetStatPercetage(EStats::Stamina, EStats::MaxStamina));

}

void UStatsComponent::RegenStamina()
{

	if (!bCanRegenStamina) return;
	Stats[EStats::Stamina] = UKismetMathLibrary::FInterpTo_Constant(
		Stats[EStats::Stamina],
		Stats[EStats::MaxStamina],
		GetWorld()->DeltaTimeSeconds,
		StaminaRgenRate
	);	
	OnStaminaPercentUpdateDelegate.Broadcast(GetStatPercetage(EStats::Stamina, EStats::MaxStamina));
}


// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();
	APlayerCharacter* OwnerCharacter = GetOwner<APlayerCharacter>();
	if (!OwnerCharacter)
	{
		return;
	}

	AttributeSet = OwnerCharacter->AttributeSet;
	if (!AttributeSet)
	{
		return;
	}

	AttributeSet->SetHealth(Stats[EStats::Health]);
	// 避免 TMap 缺失 key 触发断言；有配置就同步到 AttributeSet，没有就回填当前 AttributeSet 值。
	//const TEnumAsByte<EStats> HealthKey(EStats::Health);
	//if (const float* ConfigHealth = Stats.Find(HealthKey))
	//{
	//	AttributeSet->SetHealth(*ConfigHealth);
	//}
	//else
	//{
	//	Stats.Add(HealthKey, AttributeSet->GetHealth());
	//}
	
}

void UStatsComponent::HandleOnDeath()
{
	
	
	OnHealthZeroDelegate.Broadcast();
}


// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatsComponent::EnableRegen()
{
	UE_LOG(LogTemp, Warning, TEXT("Regen"));
	bCanRegenStamina = true;
}

float UStatsComponent::GetStatPercetage(EStats current, EStats Max)
{
	return Stats[current]/ Stats[Max];
}

void UStatsComponent::UpdateOnDetected()
{
	Stats[EStats::Detected] = UKismetMathLibrary::FInterpTo_Constant(
		Stats[EStats::Detected],
		Stats[EStats::MaxDetected],
		GetWorld()->DeltaTimeSeconds,
		Stats[EStats::DetectedRate]
	);
	OnDelectedDelegate.Broadcast(Stats[EStats::Detected] / Stats[EStats::MaxDetected]);

	if (FMath::IsNearlyEqual(Stats[EStats::Detected], Stats[EStats::MaxDetected], 0.5f))
		HasDelectedDelegate.Broadcast();
}

void UStatsComponent::UpdateOnUndetected()
{

	Stats[EStats::Detected] = UKismetMathLibrary::FInterpTo_Constant(
		Stats[EStats::Detected],
		0,
		GetWorld()->DeltaTimeSeconds,
		Stats[EStats::DetectedRate]
	);
	OnDelectedDelegate.Broadcast(Stats[EStats::Detected] / Stats[EStats::MaxDetected]);
}




