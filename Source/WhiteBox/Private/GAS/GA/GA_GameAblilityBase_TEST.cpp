// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/GA/GA_GameAblilityBase_TEST.h"

#include "Abilities/Tasks/AbilityTask_SpawnActor.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Tasks/GameplayTask_SpawnActor.h"

void UGA_GameAblilityBase_TEST::TESTAbility()
{
	UE_LOG(LogTemp, Warning, TEXT("TESTAbility Start"));

	if (!TestActor)
	{
		UE_LOG(LogTemp, Error, TEXT("TestActor class is not set"));
		return;
	}

	AActor* const Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar)
	{
		UE_LOG(LogTemp, Error, TEXT("Avatar is null (ability not bound to pawn?)"));
		return;
	}

	// UE5.5 已无 FGameplayAbilityTargetData_LocationInfoSnapshot，应使用 FGameplayAbilityTargetData_LocationInfo。
	// UAbilityTask_SpawnActor::FinishSpawningActor 会取 Data[0]：优先 HasHitResult，否则 HasEndPoint -> GetEndPointTransform()。
	// 原因：默认构造的 FGameplayAbilityTargetDataHandle 是空的；空 Handle 不会触发 Success。
	FGameplayAbilityTargetData_LocationInfo* const LocInfo = new FGameplayAbilityTargetData_LocationInfo();
	LocInfo->TargetLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
	LocInfo->TargetLocation.LiteralTransform = Avatar->GetActorTransform();
	LocInfo->SourceLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
	LocInfo->SourceLocation.LiteralTransform = Avatar->GetActorTransform();

	FGameplayAbilityTargetDataHandle Data;
	Data.Add(LocInfo);

	UAbilityTask_SpawnActor* const SpawnTask = UAbilityTask_SpawnActor::SpawnActor(this, Data, TestActor);
	if (!SpawnTask)
	{
		UE_LOG(LogTemp, Error, TEXT("UAbilityTask_SpawnActor::SpawnActor returned null"));
		return;
	}

	SpawnTask->Success.AddDynamic(this, &UGA_GameAblilityBase_TEST::SpawnSucess);
	SpawnTask->DidNotSpawn.AddDynamic(this, &UGA_GameAblilityBase_TEST::SpawnFail);

	// 原因：UAbilityTask_SpawnActor 没有实现 Activate()。ReadyForActivation() 只会走到空的 UGameplayTask::Activate()，
	// 不会自动调用 BeginSpawningActor / FinishSpawningActor（蓝图 K2Node 才会串这两步）。纯 C++ 必须手动调用，否则 Success/DidNotSpawn 都不会按预期触发。
	FGameplayAbilityTargetDataHandle DummyHandle;
	AActor* SpawnedActor = nullptr;
	if (SpawnTask->BeginSpawningActor(this, DummyHandle, TestActor, SpawnedActor))
	{
		SpawnTask->FinishSpawningActor(this, DummyHandle, SpawnedActor);
	}
}

//void UGA_GameAblilityBase_TEST::TESTGameplayTask()
//{
//	
//	UGameplayTask_SpawnActor* SpawnTask = UGameplayTask_SpawnActor::SpawnActor(
//		this,
//		GetAvatarActorFromActorInfo()->GetActorLocation(),
//		GetAvatarActorFromActorInfo()->GetActorRotation(),
//		TestActor,
//		false
//	);
//	SpawnTask->Success.AddDynamic(this, &UGA_GameAblilityBase_TEST::SpawnSucess);
//	SpawnTask->DidNotSpawn.AddDynamic(this, &UGA_GameAblilityBase_TEST::SpawnFail);
//}

void UGA_GameAblilityBase_TEST::SpawnSucess(AActor* SpawnActors)
{

	UE_LOG(LogTemp, Error, TEXT("Success!!!!!!!, name :%s"), *SpawnActors->GetName());
	UE_LOG(LogTemp, Error, TEXT("Success!!!!!!!"));
}

void UGA_GameAblilityBase_TEST::SpawnFail(AActor* SpawnActors)
{
	UE_LOG(LogTemp, Error, TEXT("fail!!!!!!!"));

}
