// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GA_GameAblilityBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FEQSFinishSignature,
	UGA_GameAblilityBase, OnEQSFinishDelegate,
	FVector, EQS_Location
);



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

	UPROPERTY(BlueprintReadOnly)
	FVector EQSQueryLocation;

	FEQSFinishSignature OnEQSFinishDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoActive = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsEnemyAbility = false;

	virtual bool DoesAbilitySatisfyTagRequirements(
		const UAbilitySystemComponent& AbilitySystemComponent,
		const FGameplayTagContainer* SourceTags,
		const FGameplayTagContainer* TargetTags,
		FGameplayTagContainer* OptionalRelevantTags) const override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GE_Self")
	float GE_Self_Magnitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GE_Self")
	TSubclassOf<UGameplayEffect>  GE_Self;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GE_Self")
	float GE_Self_Lv;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GE_Attacker")
	float GE_ToAttacker_Magnitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GE_Attacker")
	TSubclassOf<UGameplayEffect>  GE_ToAttacker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GE_Attacker")
	float GE_ToAttacker_Lv;

protected:
	class APlayerCharacter* CharRef;
	class  UAnimInstance* AnimInstance;
	UPROPERTY(BlueprintReadOnly)
	class AEnemyCharacter* EnemyCharRef;
	UPROPERTY(BlueprintReadOnly)
	class UAnimInstance* EnemyAnimInstance;
	UPROPERTY(BlueprintReadOnly)
	class AEnemyAIController* EnemyAIRef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EQS")
	UEnvQuery* QueryTemplate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EQS")
	bool bIsNeedTargetCheck{ true };
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EQS")
	TEnumAsByte<EEnvQueryRunMode::Type> EQSMode;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	void TryCacheCharacterRefs(const FGameplayAbilityActorInfo* ActorInfo);
	UFUNCTION(BlueprintCallable, Category = "EQS")
	void RunCoverEQS();

	/** EQS 成功后蓝图自行决定是否 Warp / 播 Montage 等 */
	UFUNCTION(BlueprintImplementableEvent, Category = "EQS")
	void OnCoverEQSFinished(FVector QueryLocation);

	UFUNCTION(BlueprintImplementableEvent, Category = "EQS")
	void OnCoverEQSFailed();

	virtual void OnEQSFinished(TSharedPtr<FEnvQueryResult> Result);

	/** 从 EQS 结果里按 RunMode 选取有效 Item（Score > 0）的位置 */
	bool TrySelectEQSLocation(const FEnvQueryResult& Result, FVector& OutLocation, int32& OutItemIndex) const;

	//向自己身执行GE
	UFUNCTION(BlueprintCallable)
	void ApplyGEToSelf(FGameplayTag DataTag);
	//向攻击者执行GE
	UFUNCTION(BlueprintCallable)
	void ApplyGEToAttacker(FGameplayEventData Playload, FGameplayTag DataTag);



};
