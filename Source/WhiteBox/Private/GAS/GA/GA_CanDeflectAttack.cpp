// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_CanDeflectAttack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"
#include "MotionWarpingComponent.h"
#include "GAS/Task/AbilityTask_Tick.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Player/CamerManagerComponent.h"
#include "AbilitySystemComponent.h"



void UGA_CanDeflectAttack::OnTraceHitHandle(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("UGA_CanDeflectAttack::OnTraceHitHandle BEGIN Avatar=%s Target=%s Instigator=%s"),
		*GetNameSafe(GetAvatarActorFromActorInfo()),
		*GetNameSafe(Payload.Target.Get()),
		*GetNameSafe(Payload.Instigator.Get()));

	Super::OnTraceHitHandle(Payload);

	UE_LOG(LogTemp, Warning, TEXT("UGA_CanDeflectAttack::OnTraceHitHandle AFTER Super (TakeHit/SuperParry Send should have finished)"));

	AActor* TargetActor = const_cast<AActor*>(Payload.Target.Get());
	UAbilitySystemComponent* TargetASC =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	

	if (TargetASC && TargetASC->HasMatchingGameplayTag(DeflectTag))
	{
		UE_LOG(LogTemp, Warning, TEXT(" UGA_CanDeflectAttack::OnTraceHitHandle Target: HasTag(DefleatTage)"));
		//DeflectHandler(Payload);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" UGA_CanDeflectAttack::OnTraceHitHandle Target: No HasTag(DefleatTage)"));
	}
}

void UGA_CanDeflectAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAbilityTask_WaitGameplayEvent* SuperParryEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, FGameplayTag::RequestGameplayTag("GamePlayAbility.Combat.SuperParry")
	);
	SuperParryEvent->EventReceived.AddDynamic(this, &UGA_CanDeflectAttack::DeflectHandler);
	SuperParryEvent->ReadyForActivation();

	UE_LOG(LogTemp, Warning, TEXT("UGA_CanDeflectAttack::ActivateAbility Wait SuperParry ready Avatar=%s FailAttackMontage=%s"),
		*GetNameSafe(GetAvatarActorFromActorInfo()),
		FailAttackAnimMontage ? *FailAttackAnimMontage->GetName() : TEXT("NULL"));
	GetDeflectAnim();
	WarpMontageHandler();

}

void UGA_CanDeflectAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UE_LOG(LogTemp, Warning, TEXT("UGA_CanDeflectAttack::EndAbility BEGIN Avatar=%s bWasCancelled=%d"),
		*GetNameSafe(ActorInfo && ActorInfo->AvatarActor.IsValid() ? ActorInfo->AvatarActor.Get() : nullptr),
		bWasCancelled ? 1 : 0);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (!bIsDeflectFaceToTarget)
	{
		UCamerManagerComponent* CMC = GA_TargetActor->GetComponentByClass<UCamerManagerComponent>();
		CMC->CameraPlayerFaceToTargetChangeHandle(true);
	}
	UE_LOG(LogTemp, Error, TEXT(" UGA_CanDeflectAttack::EndAbility"));
}

void UGA_CanDeflectAttack::DeflectHandler(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("UGA_CanDeflectAttack::DeflectHandler CALLED Avatar=%s Payload.Target=%s Payload.Instigator=%s FailAttackMontage=%s"),
		*GetNameSafe(GetAvatarActorFromActorInfo()),
		*GetNameSafe(Payload.Target.Get()),
		*GetNameSafe(Payload.Instigator.Get()),
		FailAttackAnimMontage ? *FailAttackAnimMontage->GetName() : TEXT("NULL"));

	AActor* TargetActor = const_cast<AActor*>(Payload.Target.Get());
	UAbilitySystemComponent* TargetASC =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	UAbilityTask_PlayMontageAndWait* FailAttackMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		"MontageAttack",
		FailAttackAnimMontage,
		FailAttackAnimMontagePlayRate,
		NAME_None,
		true,
		1.f,
		0.4f,
		false
	);
	UAbilityTask_Tick* FailAttackMontageTick = UAbilityTask_Tick::Create(this);
	if (FailAttackAnimMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("UGA_CanDeflectAttack::DeflectHandler entering FailAttackAnimMontage branch"));
		RemoveAttackMontageEndAbility();

		FailAttackMontage->OnCompleted.AddUniqueDynamic(this, &UGA_CanDeflectAttack::OnAttackCompleted);
		FailAttackMontage->OnCancelled.AddUniqueDynamic(this, &UGA_CanDeflectAttack::OnAttackCompleted);
		FailAttackMontage->OnInterrupted.AddUniqueDynamic(this, &UGA_CanDeflectAttack::OnAttackCompleted);
		FailAttackMontageTick->OnTick.AddDynamic(this, &UGA_CanDeflectAttack::UpdateDeflectAnimPlayeRate);
		FailAttackMontage->ReadyForActivation();
		FailAttackMontageTick->ReadyForActivation();
		if (GA_TargetActor)
		{
			ACharacter* TargetChar = Cast<ACharacter>(GA_TargetActor);
			UAnimInstance* TargetAnimInst = TargetChar->GetMesh()->GetAnimInstance();


			if (!bIsDeflectFaceToTarget)
			{

				UCamerManagerComponent* CMC = GA_TargetActor->GetComponentByClass<UCamerManagerComponent>();
				CMC->CameraPlayerFaceToTargetChangeHandle(false);
			}

			TargetAnimInst->Montage_Play(
				DefectAnimMontage,
				FailAttackAnimMontagePlayRate,
				EMontagePlayReturnType::Duration,
				0.4f
			);


			UMotionWarpingComponent* UWC = GetAvatarActorFromActorInfo()->GetComponentByClass<UMotionWarpingComponent>();

			FVector ActorLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
			FVector TargetLocation = GA_TargetActor->GetActorLocation();
			FTransform ActorTran = GetAvatarActorFromActorInfo()->GetActorTransform();
			FVector Directon = (ActorLocation - TargetLocation).GetSafeNormal();
			FVector WarpLoc = TargetLocation + Directon * FailAttackWarpOffset;
			FRotator WarpRot = (-Directon).Rotation();
			WarpRot.Roll = 0;
			WarpRot.Pitch = 0;

			UWC->AddOrUpdateWarpTargetFromLocationAndRotation(
				DefectWarpingName,
				WarpLoc,
				WarpRot

			);

		}










	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UGA_CanDeflectAttack::DeflectHandler SKIPPED: FailAttackAnimMontage is NULL"));
	}


}

void UGA_CanDeflectAttack::GetDeflectAnim()
{

	FFailAttackAndDeflectAnimStruct AnimStruct = DeflectAnims[0];

	FailAttackAnimMontage = AnimStruct.FailAttackAnim;
	DefectAnimMontage = AnimStruct.DefectAnim;
	FailAttackAnimMontagePlayRate = AnimStruct.FailAttackAnimPlayRate;
	FailAttackWarpOffset = AnimStruct.WarpOffset;
	DefectWarpingName = AnimStruct.WarpName;
	bIsDeflectFaceToTarget = AnimStruct.bIsFaceToTarget;
	UE_LOG(LogTemp, Warning, TEXT("UGA_CanDeflectAttack::GetDeflectAnim GOT"));
}

void UGA_CanDeflectAttack::UpdateDeflectAnimPlayeRate(float DeltaTime)
{
	ACharacter* ActorChar = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	UAnimInstance* AnimInst = ActorChar->GetMesh()->GetAnimInstance();
	float Rate = AnimInst->GetCurveValue(DeflectCurveName);

	float FinalRate = FailAttackAnimMontagePlayRate;
	FinalRate *= Rate;
	AnimInst->Montage_SetPlayRate(FailAttackAnimMontage, FinalRate);

	/*UE_LOG(LogTemp, Warning, TEXT(" UGA_CanDeflectAttack::UpdateDeflectAnimPlayeRate: PlayRate: %f , Postion: %f"), FinalRate, AnimInst->Montage_GetPosition(FailAttackAnimMontage));*/

	if(GA_TargetActor)
	{
		ACharacter* TargetChar = Cast<ACharacter>(GA_TargetActor);
		UAnimInstance* TargetAnimInst = TargetChar->GetMesh()->GetAnimInstance();
		TargetAnimInst->Montage_SetPlayRate(DefectAnimMontage, FinalRate);

	}


		
}
