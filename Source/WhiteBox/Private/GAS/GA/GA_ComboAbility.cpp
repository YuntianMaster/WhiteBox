// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_ComboAbility.h"
#include "Combat/LockComponent.h"
#include "GameFramework/Character.h"
#include "HAL/PlatformTime.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/PlayerCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "MotionWarpingComponent.h"
#include "Structure/FGASStucts.h"



UGA_ComboAbility::UGA_ComboAbility()
{
	OnGameplayAbilityCancelled.AddUObject(this, &UGA_ComboAbility::OnCancelHandle);
	
}

void UGA_ComboAbility::OnCancelHandle()
{
	CombotCounter = 0;
	BP_RemoveGameplayEffectFromOwnerWithHandle(AttackingHandle, -1);
	Cast<ACharacter>(GetAvatarActorFromActorInfo())->StopAnimMontage(CurrentMontage);
}

void UGA_ComboAbility::GA_ComboAttackHandle()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackingStart11"));
	
	if (ComboMontages.Num() == 0)
		return;
	
	UE_LOG(LogTemp, Warning, TEXT("AttackingStart"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Stats.Attacking"));
	AttackingSpecHandle = MakeOutgoingGameplayEffectSpec(GE_Attacking, 1.0f);
	AttackingHandle = K2_ApplyGameplayEffectSpecToOwner(AttackingSpecHandle);

	////Montage Play
	float AttackPlayDuration = Cast<ACharacter>(GetAvatarActorFromActorInfo())->PlayAnimMontage(ComboMontages[CombotCounter]);
	CurrentMontage = ComboMontages[CombotCounter];
	float InputTiming = 0.0f;
	float InputPassTiming = 0.0f;
	CombotCounter++;

	//WarpMotion
	ULockComponent* LockComp = GetAvatarActorFromActorInfo()->GetComponentByClass<ULockComponent>();
	if (LockComp->CurrentTargetActor)
	{
		FVector start = GetAvatarActorFromActorInfo()->GetActorLocation();
		FVector Target = LockComp->CurrentTargetActor->GetActorLocation();
		FVector Distance = Target - start;
		Distance = Distance.GetSafeNormal();
		FVector WarpTarget = Target - Distance * TargetOffset;
		float dis = FVector::Distance(start, Target);
		UMotionWarpingComponent* MotionWarpComp = GetAvatarActorFromActorInfo()->GetComponentByClass<UMotionWarpingComponent>();
		if (dis < MotionWarpDistance)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), dis);
			MotionWarpComp->OnPreUpdate.RemoveDynamic(this, &UGA_ComboAbility::OnPlayerComboMotionWarpUpdate);
			MotionWarpComp->OnPreUpdate.AddDynamic(this, &UGA_ComboAbility::OnPlayerComboMotionWarpUpdate);


		}

		else
		{
			MotionWarpComp->RemoveWarpTarget("Attack");

		}
	}



	//InputTimeing
	InputTiming = AttackPlayDuration * (1 - attackTimingPercent);
	//LockActionRef->LockInputMovement("ComboAttack");
	InputPassTiming = AttackPlayDuration + attackBufferTime;
	int CounterMax{ ComboMontages.Num() };
	CombotCounter = UKismetMathLibrary::Wrap(CombotCounter, -1, CounterMax - 1);
	////Stop input



	GetWorld()->GetTimerManager().ClearTimer(GA_FCombotInputHandle);
	GetWorld()->GetTimerManager().ClearTimer(GA_FCombotInputPassHandle);
	GetWorld()->GetTimerManager().SetTimer(
		GA_FCombotInputHandle,
		this,
		&UGA_ComboAbility::GA_FCombotInput,
		InputTiming,
		false
	);
	GetWorld()->GetTimerManager().SetTimer(
		GA_FCombotInputPassHandle,
		this,
		&UGA_ComboAbility::GA_FCombotInputPass,
		InputPassTiming,
		false
	);
	
}

void UGA_ComboAbility::OnPlayerComboMotionWarpUpdate(UMotionWarpingComponent* MWC)
{
	AActor* Avatar = GetAvatarActorFromActorInfo();


	if (!CharRef)
	{
		UE_LOG(LogTemp, Error, TEXT("UGA_ComboAbility::OnPlayerComboMotionWarpUpdate: CharRef is invaid"));
		return;
	}
	FVector ActorLocation = Avatar->GetActorLocation();
	FVector TargetLocation = CharRef->TargetActor->GetActorLocation();
	FTransform ActorTran = Avatar->GetActorTransform();
	FVector Directon = (ActorLocation - TargetLocation).GetSafeNormal();
	FVector WarpLoc = TargetLocation + Directon * ComboWarpDistanceOffset + ActorTran.TransformVector(ComboWarpOffset);
	FRotator WarpRot = (-Directon).Rotation();
	WarpRot.Roll = 0;
	WarpRot.Pitch = 0;
	
	
	MWC->AddOrUpdateWarpTargetFromLocationAndRotation(
		ComboWarpName,
		WarpLoc,
		WarpRot
		);
	//MWC->AddOrUpdateWarpTargetFromLocation(
	//	ComboWarpName,
	//	WarpLoc
	//);

	if(bComboDebugMode)
		UKismetSystemLibrary::DrawDebugSphere(
			GetWorld(),
			WarpLoc,
			50.f,
			12,
			FLinearColor::White,
			2.f,
			5.f
		);
}

void UGA_ComboAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UAbilityTask_WaitGameplayEvent* OnTraceHitSuccessTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag("Event.Combat.TraceHitSucess")

	);
	OnTraceHitSuccessTask->EventReceived.AddUniqueDynamic(this, &UGA_ComboAbility::OnComboTraceHitHandle);
	OnTraceHitSuccessTask->ReadyForActivation();
}

void UGA_ComboAbility::OnComboTraceHitHandle(FGameplayEventData Payload)
{
	const FGameplayAbilityTargetData* TargetData = Payload.TargetData.Get(0);
	const FGameplayAbilityTargetData* HitData = Payload.TargetData.Get(1);
	AActor* GAOnwer = GetAvatarActorFromActorInfo();
	if (!TargetData || TargetData->GetScriptStruct() != FTraceMontageID::StaticStruct())
	{
		UE_LOG(LogTemp, Error, TEXT("UGA_ComboAbility::OnComboTraceHitHandle: NO TraceMontageID"));
		return;
	}

	if (!HitData || HitData->GetScriptStruct() != FGameplayAbilityTargetData_SingleTargetHit::StaticStruct())
	{
		UE_LOG(LogTemp, Error, TEXT("UGA_ComboAbility::OnComboTraceHitHandle: NO Hit Result"));
		return;
	}
	//添加伤害数值
	FTraceMontageID* MontageIDTargetData = static_cast<FTraceMontageID*>(const_cast<FGameplayAbilityTargetData*>(TargetData));
	FGameplayAbilityTargetData_SingleTargetHit* HitResultData = static_cast<FGameplayAbilityTargetData_SingleTargetHit*>(const_cast<FGameplayAbilityTargetData*>(HitData));
	MontageIDTargetData->DamageAmout = DamageAmount;
	MontageIDTargetData->ToTargetGameplayCueTag = ToTargetGameplayCueTag;
	//添加GamePlayEffectContext
	FGameplayEffectContextHandle HitContextHandle = MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());
	HitContextHandle.AddInstigator(GAOnwer, GAOnwer);
	HitContextHandle.AddHitResult(HitResultData->HitResult);
	Payload.ContextHandle = HitContextHandle;
	const FHitResult Hit = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(Payload.TargetData, 1);
	AActor* HitActor = Hit.GetActor();
	if (!HitActor)
	{
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("UGA_ComboAbility::OnComboTraceHitHandle: HitActor: %s"), *HitActor->GetName());
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		HitActor,
		FGameplayTag::RequestGameplayTag("Event.Combat.TakeHit"),
		Payload
	);
}


void UGA_ComboAbility::GA_FCombotInput()
{
	
	UE_LOG(LogTemp, Warning, TEXT("Input True"));
	BP_RemoveGameplayEffectFromOwnerWithHandle(AttackingHandle,-1);
}

void UGA_ComboAbility::GA_FCombotInputPass()
{
	UE_LOG(LogTemp, Warning, TEXT("EndAblity"));
	CombotCounter = 0;
	BP_RemoveGameplayEffectFromOwnerWithHandle(AttackingHandle, -1);
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}


