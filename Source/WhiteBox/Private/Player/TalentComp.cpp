// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/TalentComp.h"
#include "PDA/PDA_Talent.h"
#include "Widget/TalentTree.h"
#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"

namespace
{
APlayerController* GetLocalPlayerControllerForTalentUI(const UActorComponent* Comp)
{
	if (!Comp || !Comp->GetWorld())
	{
		return nullptr;
	}
	APlayerController* PC = nullptr;
	if (const APawn* Pawn = Cast<APawn>(Comp->GetOwner()))
	{
		PC = Cast<APlayerController>(Pawn->GetController());
	}
	if (!PC)
	{
		PC = Comp->GetWorld()->GetFirstPlayerController();
	}
	if (PC && !PC->IsLocalPlayerController())
	{
		return nullptr;
	}
	return PC;
}

static bool CanApplyInputModes(APlayerController* PC)
{
	return IsValid(PC) && PC->GetLocalPlayer() != nullptr;
}

/** Prefer Owning Player from the widget (matches blueprint GetOwningPlayer). */
static APlayerController* ResolvePlayerForWidgetInput(UUserWidget* Widget, APlayerController* FallbackPC)
{
	if (Widget && Widget->GetOwningPlayer())
	{
		return Widget->GetOwningPlayer();
	}
	return FallbackPC;
}

static void ApplyTalentTreeInputModes(UUserWidget* TreeRoot, APlayerController* FallbackPC)
{
	if (!TreeRoot)
	{
		return;
	}

	APlayerController* InputPC = ResolvePlayerForWidgetInput(TreeRoot, FallbackPC);
	if (!CanApplyInputModes(InputPC))
	{
		UE_LOG(LogTemp, Warning, TEXT(
			"ApplyTalentTreeInputModes: skip SetInputMode (PC=%s LocalPlayer=%s). If this repeats, fix TalentTree/WBP Talent Blueprint: "
			"never Create Widget with \"World\"/null Owning Player; disconnect duplicate Set Input Mode nodes."),
			InputPC ? *InputPC->GetName() : TEXT("null"),
			(InputPC && InputPC->GetLocalPlayer()) ? TEXT("ok") : TEXT("null"));
		return;
	}

	FInputModeUIOnly UIOnly;
	UIOnly.SetWidgetToFocus(TreeRoot->TakeWidget());
	InputPC->SetInputMode(UIOnly);
	InputPC->bShowMouseCursor = true;
}
}

// Sets default values for this component's properties
UTalentComp::UTalentComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTalentComp::BeginPlay()
{
	Super::BeginPlay();
	ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
	// ...
	
}


// Called every frame
void UTalentComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTalentComp::GrantTalent(UPDA_Talent* Talent)
{
	if (!ASC || !Talent)
	{
		return;
	}
	for(TSubclassOf<UGameplayAbility> GA : Talent->GAs)
	{
		FGameplayAbilitySpecHandle GAHandle = ASC->GiveAbility(FGameplayAbilitySpec(GA, 1, 0,GetOwner()));
		GrantedTalents_GA.FindOrAdd(Talent).Add(GAHandle);
	}
	for(TSubclassOf<UGameplayEffect> GE : Talent->GEs)
	{
		FActiveGameplayEffectHandle GE_Handle = 
			ASC->ApplyGameplayEffectToSelf(GE.GetDefaultObject(), 1, ASC->MakeEffectContext());
		GrantedTalents_GE.FindOrAdd(Talent).Add(GE_Handle);
	}
	Talent->CurrentLevel = 1;
	GrantedTalents.Add(Talent);
}

void UTalentComp::GetTalent(UPDA_Talent* Talent)
{
	if (GrantedTalents_GA.Find(Talent) || GrantedTalents_GE.Find(Talent)) {
		LevelUpTalent(Talent);
	}
	else
	{
		GrantTalent(Talent);
	}

	Talent->OnTalentGet.Broadcast();
}

void UTalentComp::LevelUpTalent(UPDA_Talent* Talent)
{
	if (!ASC || !Talent)
	{
		return;
	}

	TArray<FGameplayAbilitySpecHandle>* OldGAs = GrantedTalents_GA.Find(Talent);
	TArray<FGameplayAbilitySpecHandle> NewGAs;
	if (OldGAs)
	{
		for (FGameplayAbilitySpecHandle GAHandle : *OldGAs)
		{
			FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(GAHandle);
			if (!Spec)
			{
				continue;
			}
			const int32 GA_Lv = Spec->Level;
			if (!Spec->Ability)
			{
				continue;
			}
			const TSubclassOf<UGameplayAbility> AbilityClass(Spec->Ability->GetClass());
			const int32 InputID = Spec->InputID;
			ASC->ClearAbility(GAHandle);
			const FGameplayAbilitySpecHandle NewSpecHandle =
				ASC->GiveAbility(FGameplayAbilitySpec(AbilityClass, GA_Lv + 1, InputID, GetOwner()));
			NewGAs.Add(NewSpecHandle);
		}
		GrantedTalents_GA.FindOrAdd(Talent) = MoveTemp(NewGAs);
	}

	TArray<FActiveGameplayEffectHandle>* OldGEs = GrantedTalents_GE.Find(Talent);
	TArray<FActiveGameplayEffectHandle> NewGEs;
	if (OldGEs)
	{
		for (FActiveGameplayEffectHandle GEHandle : *OldGEs)
		{
			const FActiveGameplayEffect* Active = ASC->GetActiveGameplayEffect(GEHandle);
			if (!Active)
			{
				UE_LOG(LogTemp, Warning, TEXT("Active Gameplay Effect not found for handle"));
				continue;
			}
			const UGameplayEffect* DefCDO = Active->Spec.Def;
			if (!DefCDO)
			{
				UE_LOG(LogTemp, Warning, TEXT("Gameplay Effect definition not found for active effect with handle"));
				continue;
			}

			const float CurrentLevel = Active->Spec.GetLevel();
			ASC->RemoveActiveGameplayEffect(GEHandle);
			const FActiveGameplayEffectHandle NewGe =
				ASC->ApplyGameplayEffectToSelf(DefCDO, CurrentLevel + 1.0f, ASC->MakeEffectContext());
			NewGEs.Add(NewGe);
		}
		GrantedTalents_GE.FindOrAdd(Talent) = MoveTemp(NewGEs);
	}
	Talent->CurrentLevel++;
	
}

void UTalentComp::ShowTalentUI()
{
	if (!WBP_TalentTree || !GetWorld())
	{
		return;
	}

	APlayerController* PC = GetLocalPlayerControllerForTalentUI(this);
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShowTalentUI: need local PlayerController (possessed pawn + authority)."));
		return;
	}

	// CreateWidget must use Owning PlayerController so widget GetOwningPlayer() is valid; World overload breaks BP nodes that pass GetOwningPlayer to SetInputMode.

	if (!TalentTreeWidget)
	{
		TalentTreeWidget = CreateWidget<UTalentTree>(PC, WBP_TalentTree);
		UE_LOG(LogTemp, Warning, TEXT("CreateWidget T_Tree"));
	}

	if (!TalentTreeWidget->GetOwningPlayer())
	{
		UE_LOG(LogTemp, Error, TEXT(
			"ShowTalentUI: TalentTree has no Owning Player after CreateWidget(PlayerController); input mode may fail."));
	}

	TalentTreeWidget->AddToViewport();

	// Deferred: BP Construct / child CreateWidget may finish this frame; also ensures LocalPlayer is ready.
	UUserWidget* TreeRef = TalentTreeWidget;
	TWeakObjectPtr<UUserWidget> WeakTree(TreeRef);
	TWeakObjectPtr<APlayerController> WeakPC(PC);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([WeakTree, WeakPC]()
		{
			ApplyTalentTreeInputModes(WeakTree.Get(), WeakPC.Get());
		}));
	}
}

void UTalentComp::CloseTalentUI()
{
	if (!TalentTreeWidget)
	{
		return;
	}

	APlayerController* PC = GetLocalPlayerControllerForTalentUI(this);
	APlayerController* InputPC = ResolvePlayerForWidgetInput(TalentTreeWidget, PC);
	if (InputPC && CanApplyInputModes(InputPC))
	{
		FInputModeGameOnly GameOnly;
		InputPC->SetInputMode(GameOnly);
		InputPC->bShowMouseCursor = false;
	}

	TalentTreeWidget->RemoveFromParent();
	//TalentTreeWidget = nullptr;
}

