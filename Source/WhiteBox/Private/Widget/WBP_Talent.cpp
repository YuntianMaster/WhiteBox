// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WBP_Talent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Player/PlayerCharacter.h"
#include "Player/TalentComp.h"
#include "PDA/PDA_Talent.h"


void UWBP_Talent::NativeConstruct() {
	Super::NativeConstruct();
	if (!GetOwningPlayer())
	{
		UE_LOG(LogTemp, Warning, TEXT(
			"%s has no Owning Player — TalentTree blueprint likely created this widget with World/null owner. "
			"Any Blueprint \"Set Input Mode\" using GetOwningPlayer will fail."), *GetName());
	}
	ComfirmButton->OnClicked.AddDynamic(this, &UWBP_Talent::LevelUpTalent);
	InitialUI();
	
}

void UWBP_Talent::LevelUpTalent() {

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter) {
		PlayerCharacter->TalentComp->GetTalent(TalentData);
	}

	UpdateUI();

}

void UWBP_Talent::UpdateUI()
{
	if (TalentData)
	{

		UE_LOG(LogTemp, Warning, TEXT("Update Talent UI for %s"), *TalentData->TalentName);
		TalentIcon->SetBrushFromTexture(TalentData->Icon);
		TalentName->SetText(FText::FromString(TalentData->TalentName));
		FString lv = FString::Printf(TEXT("%d/%d"), TalentData->CurrentLevel, TalentData->MaxLevel);
		TalentLevel->SetText(FText::FromString(lv));
		
	}
}

void UWBP_Talent::InitialUI()
{
	if (TalentData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Initializing Talent UI for %s"), *TalentData->TalentName);
		UpdateUI();
	}
}
