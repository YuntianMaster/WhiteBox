// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "ScalableFloat.h"
#include "FGASStucts.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct WHITEBOX_API FTraceMontageID : public FGameplayAbilityTargetData
{
    GENERATED_BODY()


    UPROPERTY(BlueprintReadWrite)
    int32 TRACE_MONTAGE_ID = INDEX_NONE;
    UPROPERTY(BlueprintReadWrite)
    float DamageAmout = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GC_Tag")
    FGameplayTag ToTargetGameplayCueTag;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GC_Tag")
    FGameplayTag ToSelfGameplayCueTag;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GC_Tag")
    bool bIsCritialHit{ false };


	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FTraceMontageID::StaticStruct();
	}
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << TRACE_MONTAGE_ID;
		Ar << DamageAmout;
		bOutSuccess = true;
		return true;
	}

};

USTRUCT(BlueprintType)
struct WHITEBOX_API FAbilityCatalogRow : public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftClassPtr<UGameplayAbility> AbilityClass;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UTexture2D> Icon;
    // ���롢��ȴ���������ݡ��� GA �ϵ� AssetTags ��ͬһ�� Tag��
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag InputTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag CooldownTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Cooldown = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayAttribute CostAttribute;
    // �츳�����Ѿ��ڰ� Spec.Level + 1 �ظ����ܣ������߰��ȼ�ȡֵ��
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FScalableFloat Cost;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftClassPtr<UGameplayEffect> CostEffectClass;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FScalableFloat Damage;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag ToTargetHitGameCue;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 DefaultLevel = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bAutoActivate = false;
  
   
};
