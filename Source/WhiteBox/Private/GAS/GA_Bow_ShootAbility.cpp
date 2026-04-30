// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_Bow_ShootAbility.h"
#include "Player/PlayerCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/GameplayAbilityTargetActor_SingleLineTrace.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_SpawnActor.h"
#include "Combat/Weapon/GeneralProjectile.h"
void UGA_Bow_ShootAbility::BowCharing()
{
	
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Stats.Drawing"));
	CharingTime = 0;
	GetWorld()->GetTimerManager().SetTimer(
		FCharingHandle,
		this,
		&UGA_Bow_ShootAbility::CharingTimeHandle,
		GetWorld()->DeltaTimeSeconds,
		true
	);

	CharRef->PlayAnimMontage(BowDrawMontage);

	FActorSpawnParameters Params;
	Params.Owner = GetAvatarActorFromActorInfo();
	const FTransform AvatarTransform = GetAvatarActorFromActorInfo()->GetActorTransform();
	AGameplayAbilityTargetActor_SingleLineTrace* TargetActor =
		GetWorld()->SpawnActor<AGameplayAbilityTargetActor_SingleLineTrace>(
			TargetClass,
			AvatarTransform,
			Params
		);
	if (!TargetActor)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
		return;
	}
	
	TargetActor->MaxRange = 9999999.f;
	TargetActor->TraceProfile = FName(TEXT("NoCollision"));
	TargetActor->StartLocation = MakeTargetLocationInfoFromOwnerActor();
	TargetActor->bTraceAffectsAimPitch = true;
	TargetActor->bDebug = true;

	UAbilityTask_WaitTargetData* WaitTargetDataTask =
		UAbilityTask_WaitTargetData::WaitTargetDataUsingActor(
			this,
			FName(TEXT("BowTargetData")),
			EGameplayTargetingConfirmation::UserConfirmed,
			TargetActor
		);

	WaitTargetDataTask->ValidData.AddDynamic(this, &UGA_Bow_ShootAbility::OnBowTargetDataValid);
	WaitTargetDataTask->ReadyForActivation();


	
}



void UGA_Bow_ShootAbility::CharingTimeHandle()
{
	CharingTime += GetWorld()->DeltaTimeSeconds;
	if (FMath::IsNearlyEqual(CharingTime, MaxCharingTime, 0.05f)) {
		CharingTime = MaxCharingTime;
		GetWorld()->GetTimerManager().ClearTimer(FCharingHandle);
	}
}


void UGA_Bow_ShootAbility::OnBowTargetDataValid(const FGameplayAbilityTargetDataHandle& Datas)
{
	//CharRef->PlayAnimMontage(BowShootMontage);
	UE_LOG(LogTemp, Warning, TEXT("OnBowTargetDataValid!!!!!!!"));
	FGameplayTagContainer AimingTag;
	AimingTag.AddTag(FGameplayTag::RequestGameplayTag("GamePlayAbility.Combat.Aim"));
	// 按标签取消「带 Aim 的瞄准 GA」。射击 GA 本身不带 Aim tag 时不会被这条扫到；参数以引擎 AbilitySystemComponent.h 为准。
	CharRef->AbilitySystemComp->CancelAbilities(&AimingTag, nullptr, this);
	if (!ShootArrowActor)
	{
		UE_LOG(LogTemp, Error, TEXT("OnBowTargetDataValid: ShootArrowActor class is null"));
		return;
	}

	FGameplayAbilityTargetData_LocationInfo* const LocInfo = new FGameplayAbilityTargetData_LocationInfo();
	LocInfo->TargetLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
	LocInfo->TargetLocation.LiteralTransform = CharRef->GetMesh()->GetSocketTransform("Arrow");
	LocInfo->SourceLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
	LocInfo->SourceLocation.LiteralTransform = CharRef->GetMesh()->GetSocketTransform("Arrow");

	FGameplayAbilityTargetDataHandle ArrowData;
	ArrowData.Add(LocInfo);

	// 生成 Transform 由 TargetData 决定（SingleLineTrace 通常为 SingleTargetHit）。蒙太奇在 FireArrow / OnArrowSpawnFailed 里再开，避免打断 Spawn 任务。
	UAbilityTask_SpawnActor* const SpawnTask = UAbilityTask_SpawnActor::SpawnActor(this, ArrowData, ShootArrowActor);
	if (!SpawnTask)
	{
		UE_LOG(LogTemp, Error, TEXT("OnBowTargetDataValid: UAbilityTask_SpawnActor::SpawnActor returned null"));
		return;
	}

	



	SpawnTask->Success.AddDynamic(this, &UGA_Bow_ShootAbility::FireArrow);
	SpawnTask->DidNotSpawn.AddDynamic(this, &UGA_Bow_ShootAbility::OnArrowSpawnFailed);
	AActor* SpawnedActor = nullptr;
	if (SpawnTask->BeginSpawningActor(this, ArrowData, ShootArrowActor, SpawnedActor))
	{
		SpawnedActor->SetInstigator(Cast<APawn>(GetAvatarActorFromActorInfo()));
		SpawnTask->FinishSpawningActor(this, ArrowData, SpawnedActor);
	}
	
	
	UE_LOG(LogTemp, Warning, TEXT("OnBowTargetDataValid:DataLoc %s"), *ArrowData.Get(0)->GetEndPoint().ToString());
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Stats.Drawing"));
	GetWorld()->GetTimerManager().ClearTimer(FCharingHandle);
}

void UGA_Bow_ShootAbility::FireArrow(AActor* SpawnedActor)
{
	if (!SpawnedActor)
	{
		UE_LOG(LogTemp, Error, TEXT("FireArrow called with null SpawnedActor"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("FireArrow:Loc %s"), *SpawnedActor->GetActorLocation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("FireArrow:Name %s"), *SpawnedActor->GetName());

	if (AGeneralProjectile* Projectile = Cast<AGeneralProjectile>(SpawnedActor))
	{
		Projectile->Char = CharRef;
		const float NormalizedCharge = FMath::Clamp(CharingTime / MaxCharingTime, 0.f, 1.f);
		Projectile->Fire(NormalizedCharge);
	}

	StartShootMontageThenReAim();
}



void UGA_Bow_ShootAbility::OnArrowSpawnFailed(AActor* SpawnedActor)
{
	(void)SpawnedActor;
	UE_LOG(LogTemp, Error, TEXT("OnArrowSpawnFailed: UAbilityTask_SpawnActor could not spawn (check authority / TargetData / class). SpawnedActor=%s"),
		SpawnedActor ? *SpawnedActor->GetName() : TEXT("nullptr"));
	StartShootMontageThenReAim();
}

void UGA_Bow_ShootAbility::StartShootMontageThenReAim()
{
	UAbilityTask_PlayMontageAndWait* ShootMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		TEXT("Shoot"),
		BowShootMontage,
		1.f,
		NAME_None,
		true,
		1.f,
		0.f,
		false
	);
	if (ShootMontage)
	{
		ShootMontage->OnCompleted.AddDynamic(this, &UGA_Bow_ShootAbility::ReAimingHandle);
		ShootMontage->ReadyForActivation();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("StartShootMontageThenReAim: montage task null"));
		ReAimingHandle();
	}
}




void UGA_Bow_ShootAbility::ReAimingHandle()
{

	UE_LOG(LogTemp, Warning, TEXT("ReAimingHandle!!!!!!!"));

	FGameplayTagContainer AimingTag;
	AimingTag.AddTag(FGameplayTag::RequestGameplayTag("GamePlayAbility.Combat.Aim"));
	CharRef->AbilitySystemComp->TryActivateAbilitiesByTag(AimingTag, true);
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}


void UGA_Bow_ShootAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if(GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Stats.Drawing")))
		GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Stats.Drawing"));
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
  