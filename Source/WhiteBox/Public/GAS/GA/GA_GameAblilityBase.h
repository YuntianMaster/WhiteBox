// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_GameAblilityBase.generated.h"


/**
 * 
 */
UCLASS()
class WHITEBOX_API UGA_GameAblilityBase : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_GameAblilityBase();
	
	UPROPERTY(Transient)
	class USkillWidget* SkillUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool bIsShowIcon = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bIsShowIcon"), Category = "UI")
	UTexture2D* SkillICon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "UI")
	bool bHasDebuff = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bHasDebuff"), Category = "UI")
	UTexture2D* DebuffIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bHasDebuff"), Category = "UI")
	TSubclassOf<UGameplayEffect> DebuffEffect;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoActive = false;
	
	



protected:
	class APlayerCharacter* CharRef;
	class  UAnimInstance* AnimInstance;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	void TryCacheCharacterRefs(const FGameplayAbilityActorInfo* ActorInfo);

};
