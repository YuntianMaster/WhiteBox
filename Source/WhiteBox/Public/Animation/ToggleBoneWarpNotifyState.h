// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ToggleBoneWarpNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UToggleBoneWarpNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Warp")
	FVector MaxRelativeLocation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Warp")
	FVector StartRelativeLocation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Warp")
	FName WarpBoneName;
	class AEnemyAIController* EnemyAIRef;
	FVector TargetBoneLoc;

	float NotifyAnimStartTime;
	float NotifyAnimCurrentTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Warp")
	float NotifyAnimCheckLastTime{2.f};



	FVector MaxBoneLoc;
	FVector StartBoneLoc;

	float MaxDist;
	float TargetDist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Warp")
	bool TrackingTarget{ true };
protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;


	bool ResolveSequenceAtMontageTime(UAnimMontage* Montage, float MontageTime, UAnimSequence*& OutSequence, float& OutSequenceTime, int32 SlotIndex = 0);
	FVector SampleBoneWorldLocation(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Anim, float TimeSeconds, FName BoneName);
	FVector SampleBoneWorldLocationFromPlayingMontage(USkeletalMeshComponent* Mesh, UAnimMontage* Montage, FName BoneName);
};
