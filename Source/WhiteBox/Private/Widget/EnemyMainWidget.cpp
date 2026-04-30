// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/EnemyMainWidget.h"
#include "Enemy/EnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "Widget/DebuffSkillWidget.h"
#include "GAS/GE/GE_GameEffectBse.h"
#include "Components/Image.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/HorizontalBox.h"


void UEnemyMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (!EnemyInstigatorActor)
	{
		UE_LOG(LogTemp, Error, TEXT("UEnemyMainWidget: EnemyInstigatorActor is not set"));
		return;
	}
	ASC = EnemyInstigatorActor->GetComponentByClass<UAbilitySystemComponent>();
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("UEnemyMainWidget: no AbilitySystemComponent on EnemyInstigatorActor"));
		return;
	}
	ASC->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("Status.Debuff")).
		AddUObject(this, &UEnemyMainWidget::UpdateDebuffHandle);
}

void UEnemyMainWidget::UpdateDebuffHandle(const FGameplayTag callback, int32 NewTag)
{

	DebuffContainer->ClearChildren();

	if (!ASC)
	{
		return;
	}
	FString TagName = callback.GetTagName().ToString();
	UE_LOG(LogTemp, Warning, TEXT("TAG [%s] NUM: %i"),*TagName, NewTag);

	
	FGameplayEffectQuery EffectQuery;
	FGameplayTagContainer Tags;
	Tags.AddTag(callback);
	EffectQuery.MakeQuery_MatchAnyOwningTags(Tags);
	
	TArray<FActiveGameplayEffectHandle> ActiveEffects = ASC->GetActiveEffects(EffectQuery);

	for (FActiveGameplayEffectHandle effect : ActiveEffects) {
		
		
		FGameplayEffectSpec GESpec =  ASC->GetActiveGameplayEffect(effect)->Spec;

			
		const UGameplayEffect* GEDef = GESpec.Def;
		UE_LOG(LogTemp, Warning, TEXT("UGameplayEffect: %s , count: %i"), *GEDef->GetName(), GESpec.GetStackCount());
		const UGE_GameEffectBse* GameEffectBse = Cast<UGE_GameEffectBse>(GEDef);
		if (!GameEffectBse)
		{
			UE_LOG(LogTemp, Warning, TEXT("CAST FAIL"));
			continue;
		}

		if (!GameEffectBse->DebuffIcon)
		{
			UE_LOG(LogTemp, Warning, TEXT("No image"));
			continue;
		}

		if (!DefaultDebuffClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("DefaultDebuffClass is not set on Enemy Main Widget"));
			continue;
		}

		UDebuffSkillWidget* debuffSkillWidget = Cast<UDebuffSkillWidget>(CreateWidget(this, DefaultDebuffClass));
		if (!debuffSkillWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("CreateWidget failed: DefaultDebuffClass is not a UDebuffSkillWidget (or class invalid)"));
			continue;
		}
		if (!debuffSkillWidget->SkillImage)
		{
			UE_LOG(LogTemp, Warning, TEXT("DebuffSkillWidget SkillImage is null: bind a widget named SkillImage in the debuff WBP (BindWidget)"));
			continue;
		}
		if (!DebuffContainer)
		{
			UE_LOG(LogTemp, Warning, TEXT("DebuffContainer (BindWidget) is null on Enemy Main Widget"));
			return;
		}

		debuffSkillWidget->ASC = ASC;
		debuffSkillWidget->SkillImage->SetBrushFromTexture(GameEffectBse->DebuffIcon);
		debuffSkillWidget->ActiveEffect = ASC->GetActiveGameplayEffect(effect);
		debuffSkillWidget->Handle = effect;
		debuffSkillWidget->UpdateDebuffSkillIconHandle();
		//debuffSkillWidget->UpdateIcon();
	
		UHorizontalBoxSlot* const DebuffSlot = DebuffContainer->AddChildToHorizontalBox(debuffSkillWidget);
		DebuffSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
		DebuffSlot->SetVerticalAlignment(VAlign_Center);
		DebuffSlot->SetHorizontalAlignment(HAlign_Left);
		DebuffSlot->SetPadding(FMargin(2.f, 0.f));
		UE_LOG(LogTemp, Warning, TEXT("Adding: %s!"),*effect.ToString());
	}
	
}

