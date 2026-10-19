#include "GAS/AbilitySystemBPLibrary.h"
#include "AbilitySystemComponent.h"
#include "Structure/FGASStucts.h"

void UAbilitySystemBPLibrary::CancelAbilitiesWithTags(
	UAbilitySystemComponent* AbilitySystemComponent,
	FGameplayTagContainer WithTags,
	UGameplayAbility* Ignore)
{
	if (!AbilitySystemComponent || WithTags.IsEmpty())
	{
		return;
	}

	AbilitySystemComponent->CancelAbilities(&WithTags, nullptr, Ignore);
}

void UAbilitySystemBPLibrary::CancelAllAbilities(
	UAbilitySystemComponent* AbilitySystemComponent,
	UGameplayAbility* Ignore)
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent->CancelAllAbilities(Ignore);
}

bool UAbilitySystemBPLibrary::GetTraceMontageFromTargetData(
	const FGameplayAbilityTargetDataHandle& TargetData,
	FTraceMontageID& OutMontageData,
	int32 Index)
{
	const FGameplayAbilityTargetData* Data = TargetData.Get(Index);
	if (!Data || Data->GetScriptStruct() != FTraceMontageID::StaticStruct())
	{
		return false;
	}

	OutMontageData = *static_cast<const FTraceMontageID*>(Data);
	return true;
}
