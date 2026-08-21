// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STT/STT_ActiveGA.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"

void USTT_ActiveGA::UnbindAbilityEnded()
{
	if (ASC)
	{
		ASC->OnAbilityEnded.RemoveAll(this);
	}
}



EStateTreeRunStatus USTT_ActiveGA::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	ActiveHandle = FGameplayAbilitySpecHandle();
	bActivated = false;

	if (!IsValid(Actor) || !AbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("STT_ActiveGA: Actor or AbilityClass is invalid."));
		return EStateTreeRunStatus::Failed;
	}

	ASC = Actor->FindComponentByClass<UAbilitySystemComponent>();
	if (!ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("STT_ActiveGA: No AbilitySystemComponent on %s."), *GetNameSafe(Actor));
		return EStateTreeRunStatus::Failed;
	}

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(AbilityClass);
	if (!Spec)
	{
		UE_LOG(LogTemp, Warning, TEXT("STT_ActiveGA: Ability %s is not granted on %s."),
			*GetNameSafe(AbilityClass), *GetNameSafe(Actor));
		return EStateTreeRunStatus::Failed;
	}

	ActiveHandle = Spec->Handle;

	// Bind before activate so sync-ending abilities are still caught.
	ASC->OnAbilityEnded.AddUObject(this, &USTT_ActiveGA::HandleAbilityEnded);

	// Already running: wait for end instead of treating as failure.
	if (Spec->IsActive())
	{
		UE_LOG(LogTemp, Warning, TEXT("STT_ActiveGA: %s already active on %s, waiting for end."),
			*GetNameSafe(AbilityClass), *GetNameSafe(Actor));
		bActivated = true;
		return EStateTreeRunStatus::Running;
	}

	bActivated = ASC->TryActivateAbility(ActiveHandle);
	if (!bActivated)
	{
		UnbindAbilityEnded();

		FGameplayTagContainer OwnedTags;
		ASC->GetOwnedGameplayTags(OwnedTags);

		UE_LOG(LogTemp, Warning,
			TEXT("STT_ActiveGA: TryActivateAbility failed for %s on %s. FailureTags=[%s] OwnedTags=[%s]"),
			*GetNameSafe(AbilityClass),
			*GetNameSafe(Actor),
			*ASC->InternalTryActivateAbilityFailureTags.ToStringSimple(),
			*OwnedTags.ToStringSimple());

		return EStateTreeRunStatus::Failed;
	}

	// Ability may have ended synchronously during TryActivateAbility.
	Spec = ASC->FindAbilitySpecFromClass(AbilityClass);
	if (!Spec || !Spec->IsActive())
	{
		UnbindAbilityEnded();
		return EStateTreeRunStatus::Succeeded;
	}

	return EStateTreeRunStatus::Running;
}

void USTT_ActiveGA::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DelayFinishHandle);
	}

	UnbindAbilityEnded();
	if (ASC && ActiveHandle.IsValid())
	{
		ASC->CancelAbilityHandle(ActiveHandle);
	}
	ActiveHandle = FGameplayAbilitySpecHandle();
	bActivated = false;

	UE_LOG(LogTemp, Warning, TEXT("USTT_ActiveGA Completed"));
	Super::ExitState(Context, Transition);
}

void USTT_ActiveGA::HandleAbilityEnded(const FAbilityEndedData& Data)
{
	if (Data.AbilitySpecHandle != ActiveHandle)
	{
		return;
	}

	if (!bIsDelayAfterGAFinish)
	{
		UnbindAbilityEnded();
		FinishTask(!Data.bWasCancelled);
		return;
	}

	bPendingFinishSucceeded = !Data.bWasCancelled;
	UnbindAbilityEnded();

	const float Delay = FMath::Max(0.f, DelayTime + FMath::RandRange(-DelayTimeOffset, DelayTimeOffset));
	GetWorld()->GetTimerManager().SetTimer(
		DelayFinishHandle,
		this,
		&USTT_ActiveGA::DelayFinishHandler,
		Delay,
		false
	);
}

void USTT_ActiveGA::DelayFinishHandler()
{
	FinishTask(bPendingFinishSucceeded);
}
