// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_WarpMontageAttack.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "Enemy/EnemyCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "MotionWarpingComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Combat/PlayerTraceComponent.h"
#include "Enemy/EnemyAIController.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Structure/FGASStucts.h"
#include "GAS/Task/AbilityTask_Tick.h"
#include "EnvironmentQuery/EnvQueryManager.h"

namespace WarpMontageAttack_Private
{

	UCharacterMovementComponent* GetMoveComp(AActor* Avatar)
	{
		return Avatar ? Avatar->GetComponentByClass<UCharacterMovementComponent>() : nullptr;
	}

	// =============================================================================
	// TEMP DEBUG ONLY — delete this function + its call site when done investigating.
	// Read-only: does not change movement mode, warp targets, or ability flow.
	// =============================================================================
	void DebugLogGroundContact(AActor* Avatar, UCharacterMovementComponent* MoveComp)
	{
		const FVector AvatarLoc = Avatar ? Avatar->GetActorLocation() : FVector::ZeroVector;

		FString CurrentFloorName = TEXT("<none>");
		FString FindFloorName = TEXT("<none>");
		float CurrentFloorDist = -1.f;
		float FindFloorDist = -1.f;
		int32 Mode = -1;
		const bool bMovingOnGround = MoveComp && MoveComp->IsMovingOnGround();

		if (MoveComp)
		{
			Mode = static_cast<int32>(MoveComp->MovementMode);

			if (MoveComp->CurrentFloor.HitResult.GetActor())
			{
				CurrentFloorName = MoveComp->CurrentFloor.HitResult.GetActor()->GetName();
				CurrentFloorDist = MoveComp->CurrentFloor.FloorDist;
			}

			if (MoveComp->UpdatedComponent)
			{
				FFindFloorResult FloorResult;
				MoveComp->FindFloor(MoveComp->UpdatedComponent->GetComponentLocation(), FloorResult, false);
				if (FloorResult.HitResult.GetActor())
				{
					FindFloorName = FloorResult.HitResult.GetActor()->GetName();
					FindFloorDist = FloorResult.FloorDist;
				}
			}
		}

		UE_LOG(LogTemp, Warning,
			TEXT("[WarpMontageGroundDebug] Avatar=%s Loc=%s Mode=%d IsMovingOnGround=%s CurrentFloor=%s (Dist=%.2f) FindFloor=%s (Dist=%.2f)"),
			*GetNameSafe(Avatar),
			*AvatarLoc.ToCompactString(),
			Mode,
			bMovingOnGround ? TEXT("true") : TEXT("false"),
			*CurrentFloorName,
			CurrentFloorDist,
			*FindFloorName,
			FindFloorDist);
	}
	// =============================================================================
	// END TEMP DEBUG
	// =============================================================================
}
// 播放需要Warping的动画
void UGA_WarpMontageAttack::WarpMontageHandler()
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar || !AttackMontage)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		return;

	}

	if (!GroundCheckHandle())
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		return;

	}
	StartMovementModeChangeHandle();
	bEndWarpStarted = false;

	// TEMP DEBUG — delete this call (+ DebugLogGroundContact above) later
	//WarpMontageAttack_Private::DebugLogGroundContact(Avatar, MoveComp);
	MWcomp = Avatar->GetComponentByClass<UMotionWarpingComponent>();
	MWcomp->AddOrUpdateWarpTargetFromLocation(EndWarpingName, Avatar->GetActorLocation());
	if (bTrackTarget)
	{
		WarpTarget_TRACKING();

	}

	else
	{
		WarpTarget_NOTRACKING();
	}

	ACharacter* Character = Cast<ACharacter>(Avatar);
	FAnimMontageInstance* Instance = Character->GetMesh()->GetAnimInstance()->GetActiveInstanceForMontage(AttackMontage);
	MontageInstanceID = Instance ? Instance->GetInstanceID() : INDEX_NONE;

	UPlayerTraceComponent* PlayerTraceComponent = Avatar->GetComponentByClass<UPlayerTraceComponent>();
	if (PlayerTraceComponent)
	{
		PlayerTraceComponent->Trace_MontageInstanceID = MontageInstanceID;
	}

}
// 播放不需要Warping的动画
void UGA_WarpMontageAttack::NoWarpMontageHandle()
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar || !AttackMontage)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		return;
	}
	if (!GroundCheckHandle())
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		return;

	}
	StartMovementModeChangeHandle();
	
	UAbilityTask_PlayMontageAndWait* MontageAttackTask =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"MontageAttack",
			AttackMontage,
			PlayRate,
			NAME_None,
			true,
			1.f,
			0.f,
			false
		);

	if (!MontageAttackTask)
	{
		return;
	}

	MontageAttackTask->ReadyForActivation();
	UE_LOG(LogTemp, Warning, TEXT("UGA_WarpMontageAttack::NoWarpMontageHandle running!"));

	MontageAttackTask->OnCompleted.AddUniqueDynamic(this, &UGA_WarpMontageAttack::OnAttackCompleted);
	MontageAttackTask->OnBlendOut.AddUniqueDynamic(this, &UGA_WarpMontageAttack::OnAttackCompleted);
	MontageAttackTask->OnCancelled.AddUniqueDynamic(this, &UGA_WarpMontageAttack::OnAttackCompleted);
	MontageAttackTask->OnInterrupted.AddUniqueDynamic(this, &UGA_WarpMontageAttack::OnAttackCompleted);

	ACharacter* Character = Cast<ACharacter>(Avatar);
	FAnimMontageInstance* Instance = Character->GetMesh()->GetAnimInstance()->GetActiveInstanceForMontage(AttackMontage);
	MontageInstanceID = Instance ? Instance->GetInstanceID() : INDEX_NONE;

	UPlayerTraceComponent* PlayerTraceComponent = Avatar->GetComponentByClass<UPlayerTraceComponent>();
	if (PlayerTraceComponent)
	{
		PlayerTraceComponent->Trace_MontageInstanceID = MontageInstanceID;
		PlayerTraceComponent->GA_WarpMontageAttack = this;
	}
}

void UGA_WarpMontageAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetWorld()->GetTimerManager().ClearTimer(WaitTimeHandle);
	AActor* Avatar = GetAvatarActorFromActorInfo();
	ACharacter* Character = Cast<ACharacter>(Avatar);
	if (Character)
		Character->MovementModeChangedDelegate.RemoveDynamic(this, &UGA_WarpMontageAttack::WaitMovementModeHandle);
	// MWcomp may still be null if EndAbility is called before WarpMontageHandler
	// (e.g. bIsNeedGround early-out).
	if (MWcomp)
	{
		MWcomp->RemoveWarpTarget(AttackWarpingName);
		MWcomp->RemoveWarpTarget(EndWarpingName);
		MWcomp = nullptr;
	}

	if (Avatar)
	{
		if (UPlayerTraceComponent* PlayerTraceComponent = Avatar->GetComponentByClass<UPlayerTraceComponent>())
		{
			PlayerTraceComponent->GA_WarpMontageAttack = nullptr;
		}
	}
}

void UGA_WarpMontageAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	// Do not SetMovementMode(MOVE_Flying) here.
	// WarpMontageHandler may still need a ground check; flying first makes IsMovingOnGround always false.

	//接收TraceComp的碰撞结果
	UAbilityTask_WaitGameplayEvent* TraceHitSuccess = 
		UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,FGameplayTag::RequestGameplayTag("Event.Combat.TraceHitSucess"));

	TraceHitSuccess->EventReceived.AddDynamic(this, &UGA_WarpMontageAttack::OnTraceHitHandle);
	TraceHitSuccess->ReadyForActivation();

	//注册TickTask
	UAbilityTask_Tick* TickTask = UAbilityTask_Tick::Create(this);
	if(bIsNeedCurveMontagePlayRate)
	{
		TickTask->OnTick.AddDynamic(this, &UGA_WarpMontageAttack::UpdatePlayeRate);
		TickTask->ReadyForActivation();
	}
}


void UGA_WarpMontageAttack::OnTraceHitHandle(FGameplayEventData Payload)
{

	OnTraceSuccessBroadCast();
	
	const FTraceMontageID* Data =
		static_cast<const FTraceMontageID*>(Payload.TargetData.Get(0));
	Payload.EventMagnitude = DamageMagnitude;

	FGameplayTag HitTag = 
		bIsCriticalHit ? FGameplayTag::RequestGameplayTag("Event.Combat.TakeCriticalHit") : FGameplayTag::RequestGameplayTag("Event.Combat.TakeHit");
	AActor* TargetActor = const_cast<AActor*>(Payload.Target.Get());
	if (MontageInstanceID == Data->TRACE_MONTAGE_ID)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			TargetActor,
			HitTag,
			Payload
		);

	}
	UE_LOG(LogTemp, Error, TEXT("Payload ID: %i"), Data->TRACE_MONTAGE_ID);
	UE_LOG(LogTemp, Warning, TEXT("Payload ID: %i"),MontageInstanceID);
		
}

void UGA_WarpMontageAttack::UpdatePlayeRate(float DeltaTime)
{
	ACharacter* ActorChar = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	UAnimInstance* AnimInst = ActorChar->GetMesh()->GetAnimInstance();
	if (!AnimInst->Montage_IsPlaying(AttackMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("UGA_WarpMontageAttack::UpdatePlayeRate: Montage is not playing"));
		return;
	}

	if (!AttackMontage->HasCurveData(PlayRateCurve)) {

		UE_LOG(LogTemp, Warning, TEXT("UGA_WarpMontageAttack::UpdatePlayeRate: PlayRateCurve is not exist"));
		return;
	}
	
	float Rate = AnimInst->GetCurveValue(PlayRateCurve);
	UE_LOG(LogTemp, Warning, TEXT("UGA_WarpMontageAttack::UpdatePlayeRate: PlayRateCurve value : %f"), Rate);
	Rate *= PlayRate;
	AnimInst->Montage_SetPlayRate(AttackMontage, Rate);

}


void UGA_WarpMontageAttack::OnAttackCompleted()
{
	OnAttackCompletedBP();

	if (AActor* Avatar = GetAvatarActorFromActorInfo())
	{
		if (UCharacterMovementComponent* MoveComp = Avatar->GetComponentByClass<UCharacterMovementComponent>())
		{
			//// Prefer Walking when floor is present; Forced Falling leaves Mode stuck
			//// and makes the next IsMovingOnGround check fail while visually grounded.
			//if (WarpMontageAttack_Private::IsAvatarPhysicallyOnGround(MoveComp))
			//{
			//	MoveComp->SetMovementMode(MOVE_Walking);
			//}
			//else
			{

				EndMovementModeChangeHandle();

				ACharacter* Character = Cast<ACharacter>(Avatar);
				if(bIsNeedWaitEndMovementMode)
				{
					EMovementMode CurrentMode = Character->GetCharacterMovement()->MovementMode;
					if (CurrentMode == WaitMovementMode)
					{
						EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
						return;
					}
					Character->MovementModeChangedDelegate.AddDynamic(this, &UGA_WarpMontageAttack::WaitMovementModeHandle);
					GetWorld()->GetTimerManager().ClearTimer(WaitTimeHandle);

					GetWorld()->GetTimerManager().SetTimer(
						WaitTimeHandle,
						this,
						&UGA_WarpMontageAttack::WaitMovementModeTimeHandle,
						WaitTime,
						false
					);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Attack Completed!"));
					EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
				}

			}
		}
	}


}

void UGA_WarpMontageAttack::WaitMovementModeHandle(ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	

	EMovementMode CurrentMode = Character->GetCharacterMovement()->MovementMode;
	if(CurrentMode == WaitMovementMode)
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);

}

void UGA_WarpMontageAttack::WaitMovementModeTimeHandle()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UGA_WarpMontageAttack::WarpTarget_NOTRACKING()
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	UPlayerTraceComponent* PlayerTraceComponent = Avatar->GetComponentByClass<UPlayerTraceComponent>();
	FVector ActorLocation = Avatar->GetActorLocation();
	FVector EndLocation = ActorLocation + Avatar->GetActorRotation().RotateVector(EndWarpingLocation);
	UAbilityTask_PlayMontageAndWait* MontageAttackTask =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"MontageAttack",
			AttackMontage,
			PlayRate,
			NAME_None,
			true,
			1.f,
			0.f,
			false
		);

	if (!MWcomp)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMotionWarpingComponent is not exist!"));
		return;
	}


	AActor* TargetActor = EnemyAIRef->EnemyTargetActor;
	if (!TargetActor)
	{

		UE_LOG(LogTemp, Warning, TEXT("UGA_WarpMontageAttack: Enemy No Target"));
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		return;
	}



	FVector TargetLocation = TargetActor->GetActorLocation();
	bIsActorTarget ? TargetLocation : TargetLocation = EQSQueryLocation;
	FTransform ActorTran = Avatar->GetActorTransform();
	FVector Directon = (ActorLocation - TargetLocation).GetSafeNormal();
	FVector WarpLoc = TargetLocation + Directon * WarpDistanceOffset + ActorTran.TransformVector(WarpOffset);


	if (EQSQueryLocation.IsZero()&&!bIsActorTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("EQSQueryLocation is zero!"));
		return;
	}

	FRotator WarpRot = (-Directon).Rotation();
	WarpRot.Roll = 0;
	WarpRot.Pitch = 0;

	UE_LOG(LogTemp, Warning, TEXT("Attacking!"));
	MWcomp->AddOrUpdateWarpTargetFromLocationAndRotation(AttackWarpingName, WarpLoc, WarpRot);
	MontageAttackTask->ReadyForActivation();
	if (PlayerTraceComponent)
	{
		PlayerTraceComponent->GA_WarpMontageAttack = this;
	}
	MontageAttackTask->OnCompleted.AddUniqueDynamic(this, &UGA_WarpMontageAttack::OnAttackCompleted);
	MontageAttackTask->OnBlendOut.AddUniqueDynamic(this, &UGA_WarpMontageAttack::OnAttackCompleted);
	MontageAttackTask->OnCancelled.AddUniqueDynamic(this, &UGA_WarpMontageAttack::OnAttackCompleted);
	MontageAttackTask->OnInterrupted.AddUniqueDynamic(this, &UGA_WarpMontageAttack::OnAttackCompleted);
	//！没有角色返回逻辑！
	
}

bool UGA_WarpMontageAttack::GroundCheckHandle()
{
	if (bIsNeedGroundCheck)
	{
		AActor* Avatar = GetAvatarActorFromActorInfo();
		if (!Avatar || !AttackMontage)
		{
			
			return false;
		}

		UCharacterMovementComponent* MoveComp = Avatar->GetComponentByClass<UCharacterMovementComponent>();

		if (bIsNeedGround && MoveComp && !MoveComp->IsMovingOnGround())
		{
			
			return false;
		}

		if (!bIsNeedGround && MoveComp && MoveComp->IsMovingOnGround())
		{

			return false;
		}
	}

	return true;

}



void UGA_WarpMontageAttack::StartMovementModeChangeHandle()
{
	if (bIsNeedchangeMovementMode)

	{

		AActor* Avatar = GetAvatarActorFromActorInfo();
		if (!Avatar || !AttackMontage)
		{
			EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
			return;
		}

		UCharacterMovementComponent* MoveComp = Avatar->GetComponentByClass<UCharacterMovementComponent>();

		MoveComp->SetMovementMode(StartMovementMode);
	}

}

void UGA_WarpMontageAttack::EndMovementModeChangeHandle()
{
	if (bIsNeedchangeMovementMode)

	{

		AActor* Avatar = GetAvatarActorFromActorInfo();
		if (!Avatar || !AttackMontage)
		{
			EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
			return;
		}

		UCharacterMovementComponent* MoveComp = Avatar->GetComponentByClass<UCharacterMovementComponent>();

		MoveComp->SetMovementMode(EndMovementMode);
	}
}



void UGA_WarpMontageAttack::OnMotionWarpUpdate(UMotionWarpingComponent* UWC)
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!EnemyAIRef)
	{

		UE_LOG(LogTemp, Warning, TEXT("UGA_WarpMontageAttack: EnemyAIRef is not found"));
		return;
	}

	AActor* TargetActor = EnemyAIRef->EnemyTargetActor;
	if (!TargetActor)
	{

		UE_LOG(LogTemp, Warning, TEXT("UGA_WarpMontageAttack: Enemy No Target"));
		return;
	}

	FVector ActorLocation = Avatar->GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();
	bIsActorTarget ? TargetLocation : TargetLocation = EQSQueryLocation;
	FTransform ActorTran = Avatar->GetActorTransform();
	FVector Directon = (ActorLocation - TargetLocation).GetSafeNormal();
	FVector WarpLoc = TargetLocation + Directon * WarpDistanceOffset + ActorTran.TransformVector(WarpOffset);
	FRotator WarpRot = (-Directon).Rotation();
	WarpRot.Roll = 0;
	WarpRot.Pitch = 0;
	UE_LOG(LogTemp, Warning, TEXT("WarpLoc: %s!"),*WarpLoc.ToString());

	UWC->AddOrUpdateWarpTargetFromLocationAndRotation(
		AttackWarpingName,
		WarpLoc,
		WarpRot

	);

	if (!bEndWarpStarted && UWC) {
		for (URootMotionModifier* Modifier : UWC->GetModifiers()) {
			const URootMotionModifier_Warp* Warp = Cast<URootMotionModifier_Warp>(Modifier);
			if (!Warp || Warp->WarpTargetName != EndWarpingName)
				continue;
			if (Warp->GetState() == ERootMotionModifierState::Active)
			{
				bEndWarpStarted = true;
				if (Avatar)
				{

					FVector EndLocation = ActorLocation + Avatar->GetActorRotation().RotateVector(EndWarpingLocation);
					UWC->AddOrUpdateWarpTargetFromLocation(EndWarpingName, EndLocation);
				}

			}


		}


	

	}

}


void UGA_WarpMontageAttack::WarpTarget_TRACKING()
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	UPlayerTraceComponent* PlayerTraceComponent = Avatar->GetComponentByClass<UPlayerTraceComponent>();
	if(!MWcomp)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMotionWarpingComponent is not exist!"));
		return;
	}
	MWcomp->OnPreUpdate.RemoveDynamic(this, &UGA_WarpMontageAttack::OnMotionWarpUpdate);
	MWcomp->OnPreUpdate.AddDynamic(this, &UGA_WarpMontageAttack::OnMotionWarpUpdate);
	FVector ActorLocation = Avatar->GetActorLocation();

	UE_LOG(LogTemp, Warning, TEXT("Attacking start!"));
	UAbilityTask_PlayMontageAndWait* MontageAttackTask =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			"MontageAttack",
			AttackMontage,
			PlayRate,
			NAME_None,
			true,
			1.f,
			0.f,
			false
		);


	MontageAttackTask->ReadyForActivation();
	if (PlayerTraceComponent)
	{
		PlayerTraceComponent->GA_WarpMontageAttack = this;
	}
	MontageAttackTask->OnCompleted.AddUniqueDynamic(this, &UGA_WarpMontageAttack::OnAttackCompleted);
	MontageAttackTask->OnBlendOut.AddUniqueDynamic(this, &UGA_WarpMontageAttack::OnAttackCompleted);
	MontageAttackTask->OnCancelled.AddUniqueDynamic(this, &UGA_WarpMontageAttack::OnAttackCompleted);
	MontageAttackTask->OnInterrupted.AddUniqueDynamic(this, &UGA_WarpMontageAttack::OnAttackCompleted);

}
