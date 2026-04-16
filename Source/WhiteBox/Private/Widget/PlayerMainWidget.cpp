// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PlayerMainWidget.h"
#include "Widget/SkillWidget.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Player/PlayerCharacter.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GAS/GA/GA_GameAblilityBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogWB_PlayerHUD, Log, All);

void UPlayerMainWidget::UpdatePlayerHUD()
{
	APlayerCharacter* CharRef = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (!CharRef)
	{
		UE_LOG(LogWB_PlayerHUD, Warning, TEXT("UpdatePlayerHUD: GetOwningPlayerPawn 不是 APlayerCharacter 或为 null（%s）"),
			*GetName());
		return;
	}
	if (!CharRef->AbilitySystemComp)
	{
		UE_LOG(LogWB_PlayerHUD, Warning, TEXT("UpdatePlayerHUD: CharRef->AbilitySystemComp 为 null（角色 %s，Widget %s）"),
			*CharRef->GetName(), *GetName());
		return;
	}
	if (!SkillContainer)
	{
		UE_LOG(LogWB_PlayerHUD, Warning, TEXT("UpdatePlayerHUD: SkillContainer 未绑定（BindWidget），检查 PlayerMainWidget UMG（%s）"),
			*GetName());
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AllAbilitiesSpecHandles;
	CharRef->AbilitySystemComp->GetAllAbilities(AllAbilitiesSpecHandles);
	SkillContainer->ClearChildren();

	int32 AddedCount = 0;
	int32 SkipNoSpec = 0;
	int32 SkipWrongClass = 0;
	int32 SkipNoSkillUI = 0;

	for (const FGameplayAbilitySpecHandle& AbilityHandle : AllAbilitiesSpecHandles)
	{
		FGameplayAbilitySpec* Spec =
			CharRef->AbilitySystemComp->FindAbilitySpecFromHandle(AbilityHandle);
		if (!Spec || !Spec->Ability)
		{
			++SkipNoSpec;
			UE_LOG(LogWB_PlayerHUD, Verbose, TEXT("  跳过 Handle：找不到 Spec 或 Ability 为空"));
			continue;
		}

		UGA_GameAblilityBase* GameAbility = Cast<UGA_GameAblilityBase>(Spec->Ability);
		if (!GameAbility)
		{
			++SkipWrongClass;
			UE_LOG(LogWB_PlayerHUD, Verbose, TEXT("  跳过非 UGA_GameAblilityBase：%s（CDO类 %s）"),
				*Spec->Ability->GetName(), *Spec->Ability->GetClass()->GetName());
			continue;
		}
		if (!GameAbility->SkillICon)
		{
			++SkipNoSkillUI;
			UE_LOG(LogWB_PlayerHUD, Log, TEXT("  跳过（无 SkillICon）：实例 %s，类 %s（本地未创建 UI 或非本机能力等）"),
				*GameAbility->GetName(), *GameAbility->GetClass()->GetName());
			continue;
		}

		if (!GameAbility->bIsShowIcon)
		{

			continue;
		}
		USkillWidget* AbilitySkill = TryCreateSkillUI();
		if (!AbilitySkill)
		{
			UE_LOG(LogWB_PlayerHUD, Warning, TEXT("  跳过：未能创建技能 Widget（%s）"), *GameAbility->GetName());
			continue;
		}
		if (!AbilitySkill->SkillImage)
		{
			UE_LOG(LogWB_PlayerHUD, Warning, TEXT("  跳过：技能 Widget 未绑定 SkillImage（%s，检查 WBP_Skill）"),
				*GameAbility->GetName());
			continue;
		}
		UHorizontalBoxSlot* const SkillSlot = SkillContainer->AddChildToHorizontalBox(AbilitySkill);
		if (SkillSlot)
		{
			SkillSlot->SetVerticalAlignment(VAlign_Center);
			SkillSlot->SetHorizontalAlignment(HAlign_Center);
		}
		AbilitySkill->AbilitySpec = AbilityHandle;
		AbilitySkill->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		AbilitySkill->SkillImage->SetBrushFromTexture(GameAbility->SkillICon);
		
		++AddedCount;
		UE_LOG(LogWB_PlayerHUD, Log, TEXT("  已加入技能条：%s（Widget=%s）"),
			*GameAbility->GetName(), *AbilitySkill->GetName());
	}

	UE_LOG(LogWB_PlayerHUD, Log, TEXT("UpdatePlayerHUD 结束：ASC 能力数=%d，加入条=%d，无Spec=%d，非基类能力=%d，无SkillUI=%d（Widget=%s）"),
		AllAbilitiesSpecHandles.Num(), AddedCount, SkipNoSpec, SkipWrongClass, SkipNoSkillUI, *GetName());
}


USkillWidget* UPlayerMainWidget::TryCreateSkillUI()
{
	

	if (!DefaultSkillClass)
	{
		UE_LOG(LogTemp, Error, TEXT("DefaultSkillClass 未设置，无法在 PIE 中创建技能 UI。请在能力蓝图（继承 %s 的子类）的 Default Skill Class 中指定 USkillWidget 的 Widget 蓝图类。"),
			*GetClass()->GetName());
		return nullptr;
	}

	// 作为本 Widget 的子树创建，避免仅挂在 PC 下时与面板布局/可见性不一致。
	USkillWidget* SkillUI = Cast<USkillWidget>(CreateWidget(this, DefaultSkillClass));

	return SkillUI;
	

}