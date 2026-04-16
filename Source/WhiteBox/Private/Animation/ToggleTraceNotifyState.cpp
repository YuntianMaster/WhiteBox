// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ToggleTraceNotifyState.h"
#include "Combat/PlayerTraceComponent.h"

void UToggleTraceNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

	if (!MeshComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("NotifyBegin: MeshComp is nullptr"));
		return;
	}
	UWorld* World = MeshComp->GetWorld();
	if (!World || !World->IsGameWorld())
	{
		// ¹Ø¼üÐÞ¸´µã£¡£¡·ÀÖ¹±à¼­Æ÷Ô¤ÀÀ´¥·¢±ÀÀ£
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("NotifyBegin: MeshComp has no Owner"));
		return;
	}

	UPlayerTraceComponent* TraceComp{

		MeshComp->GetOwner()->FindComponentByClass<UPlayerTraceComponent>()

	};

	if (!IsValid(TraceComp)) return;
	TraceComp->bIsAttacking = true;
}

void UToggleTraceNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("NotifyBegin: MeshComp is nullptr"));
		return;
	}
	UWorld* World = MeshComp->GetWorld();
	if (!World || !World->IsGameWorld())
	{
		// ¹Ø¼üÐÞ¸´µã£¡£¡·ÀÖ¹±à¼­Æ÷Ô¤ÀÀ´¥·¢±ÀÀ£
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("NotifyBegin: MeshComp has no Owner"));
		return;
	}
	UPlayerTraceComponent* TraceComp{

		MeshComp->GetOwner()->FindComponentByClass<UPlayerTraceComponent>()

	};

	if (!IsValid(TraceComp)) return;
	TraceComp->bIsAttacking = false;

}
