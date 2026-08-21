// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STT/STT_TurnToLocation.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "MotionWarpingComponent.h"

namespace
{
	const FName TurnWarpTargetName(TEXT("TurnInplace"));
}

void USTT_TurnToLocation::CleanupWarpTarget()
{
	if (!IsValid(ActorRef))
	{
		return;
	}

	if (UMotionWarpingComponent* Comp = ActorRef->FindComponentByClass<UMotionWarpingComponent>())
	{
		Comp->RemoveWarpTarget(TurnWarpTargetName);
	}
}

void USTT_TurnToLocation::ClearMontageEndDelegate()
{
	if (UAnimInstance* AnimIns = CachedAnimInstance.Get())
	{
		if (UAnimMontage* Montage = CachedTurnMontage.Get())
		{
			FOnMontageEnded EmptyDelegate;
			AnimIns->Montage_SetEndDelegate(EmptyDelegate, Montage);
		}
	}

	CachedAnimInstance.Reset();
	CachedTurnMontage.Reset();
}

bool USTT_TurnToLocation::TurnToLocation()
{
	if (!IsValid(ActorRef))
	{
		return false;
	}

	FVector ActorLocation = ActorRef->GetActorLocation();
	const float TargetYaw = UKismetMathLibrary::FindLookAtRotation(ActorLocation, DestinationLocation).Yaw;
	const float YawOffset = UKismetMathLibrary::NormalizeAxis(TargetYaw - ActorRef->GetActorRotation().Yaw);

	ACharacter* Character = Cast<ACharacter>(ActorRef);
	if (!Character)
	{
		return false;
	}

	UAnimInstance* AnimIns = Character->GetMesh() ? Character->GetMesh()->GetAnimInstance() : nullptr;

	if (UMotionWarpingComponent* MotionWarpingComp = Character->FindComponentByClass<UMotionWarpingComponent>())
	{
		MotionWarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(
			TurnWarpTargetName,
			Character->GetActorLocation(),
			FRotator(0.f, TargetYaw, 0.f));
	}

	UAnimMontage* MontageToPlay = nullptr;
	if (YawOffset < 0.f && YawOffset >= -135.f)
	{
		MontageToPlay = L_90;
	}
	else if (YawOffset < -135.f)
	{
		MontageToPlay = L_180;
	}
	else if (YawOffset > 0.f && YawOffset <= 135.f)
	{
		MontageToPlay = R_90;
	}
	else if (YawOffset > 135.f)
	{
		MontageToPlay = R_180;
	}

	if (!MontageToPlay || !AnimIns)
	{
		CleanupWarpTarget();
		return false;
	}

	Character->PlayAnimMontage(MontageToPlay);

	CachedAnimInstance = AnimIns;
	CachedTurnMontage = MontageToPlay;

	TWeakObjectPtr<USTT_TurnToLocation> WeakThis(this);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([WeakThis](UAnimMontage* Montage, bool bInterrupted)
	{
		USTT_TurnToLocation* StrongThis = WeakThis.Get();
		if (!IsValid(StrongThis))
		{
			return;
		}

		StrongThis->CachedAnimInstance.Reset();
		StrongThis->CachedTurnMontage.Reset();
		StrongThis->CleanupWarpTarget();

		// 正常播完：结束 STT；被打断时由 ExitState 处理，避免重复 FinishTask
		if (!bInterrupted)
		{
			StrongThis->FinishTask(true);
		}
	});
	AnimIns->Montage_SetEndDelegate(EndDelegate, MontageToPlay);

	return true;
}

EStateTreeRunStatus USTT_TurnToLocation::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	if (TurnToLocation())
	{
		return EStateTreeRunStatus::Running;
	}
		return EStateTreeRunStatus::Succeeded;
}

void USTT_TurnToLocation::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	// 先解绑，避免 Exit 后 Montage 结束回调打到已失效的 Task
	ClearMontageEndDelegate();
	CleanupWarpTarget();
	Super::ExitState(Context, Transition);


}
