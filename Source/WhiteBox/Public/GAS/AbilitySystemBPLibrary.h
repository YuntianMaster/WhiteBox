#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "Structure/FGASStucts.h"
#include "AbilitySystemBPLibrary.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;

/**
 * Blueprint wrappers for ASC APIs that are C++-only in engine.
 */
UCLASS()
class WHITEBOX_API UAbilitySystemBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Cancel active abilities whose Ability Tags match WithTags. */
	UFUNCTION(BlueprintCallable, Category = "Ability|Cancel")
	static void CancelAbilitiesWithTags(
		UAbilitySystemComponent* AbilitySystemComponent,
		UPARAM(meta = (Categories = "GamePlayAbility")) FGameplayTagContainer WithTags,
		UGameplayAbility* Ignore = nullptr
	);

	/** Cancel all active abilities on this ASC. */
	UFUNCTION(BlueprintCallable, Category = "Ability|Cancel")
	static void CancelAllAbilities(
		UAbilitySystemComponent* AbilitySystemComponent,
		UGameplayAbility* Ignore = nullptr
	);

	/** Index 0 is FTraceMontageID on combat hit payloads. */
	UFUNCTION(BlueprintCallable, Category = "Ability|TargetData", meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool GetTraceMontageFromTargetData(
		const FGameplayAbilityTargetDataHandle& TargetData,
		FTraceMontageID& OutMontageData,
		int32 Index = 0
	);
};
