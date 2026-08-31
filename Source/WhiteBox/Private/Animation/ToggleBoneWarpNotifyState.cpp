// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ToggleBoneWarpNotifyState.h"
#include "Animation/PlayerAnimInstance.h"
#include "Structure/FAnimInstStruct.h"
#include "Enemy/EnemyAIController.h"

void UToggleBoneWarpNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	EnemyAIRef = nullptr;
	TargetBoneLoc = FVector::ZeroVector;

	if (!MeshComp)
	{
		return;
	}

	UWorld* World = MeshComp->GetWorld();
	if (!World || !World->IsGameWorld())
	{
		// Persona / ???????? Notify???? AI Controller
		return;
	}
	



	APawn* Pawn = Cast<APawn>(MeshComp->GetOwner());
	if (!Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UToggleBoneWarpNotifyState::NotifyBegin : Owner is not a Pawn"));
		return;
	}

	EnemyAIRef = Cast<AEnemyAIController>(Pawn->GetController());
	if (!EnemyAIRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("UToggleBoneWarpNotifyState::NotifyBegin : EnemyAIRef is Invalid"));
		return;
	}

	if (!EnemyAIRef->EnemyTargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("UToggleBoneWarpNotifyState::NotifyBegin : EnemyTargetActor is Invalid"));
		EnemyAIRef = nullptr;
		return;
	}

	TargetBoneLoc = EnemyAIRef->EnemyTargetActor->GetActorLocation();
	UAnimInstance* AnimInst = MeshComp->GetAnimInstance();
	UAnimMontage* Montage = Cast<UAnimMontage>(Animation);
	NotifyAnimStartTime = AnimInst->Montage_GetPosition(Montage);
	StartBoneLoc = SampleBoneWorldLocation(MeshComp, Animation, NotifyAnimStartTime, WarpBoneName);
	MaxDist = 0;

	for (int i = 0; i < 30; i++) {

		float checktime = NotifyAnimStartTime + (NotifyAnimCheckLastTime) / 30 * i;
		FVector CheckLoc = SampleBoneWorldLocation(MeshComp, Animation, checktime, WarpBoneName);

		if (FVector::Distance(CheckLoc, StartBoneLoc) > MaxDist) {

			MaxDist = FVector::Distance(CheckLoc, StartBoneLoc);
			MaxBoneLoc = CheckLoc;
		}
	}

	TargetDist = FVector::Distance(TargetBoneLoc, MeshComp->GetOwner()->GetActorLocation());
	NotifyAnimCurrentTime = NotifyAnimStartTime;
}


void UToggleBoneWarpNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	if (!MeshComp)
	{
		return;
	}

	UWorld* World = MeshComp->GetWorld();
	if (!World || !World->IsGameWorld())
	{
		return;
	}

	UPlayerAnimInstance* Anim = Cast<UPlayerAnimInstance>(MeshComp->GetAnimInstance());
	if (!Anim)
	{
		return;
	}

	if (!EnemyAIRef || !EnemyAIRef->EnemyTargetActor)
	{
		if (FAnimInstStruct* Entry = Anim->BoneWarpMap.Find(WarpBoneName))
		{
			Entry->BoneWarpActive = false;
		}
		return;
	}

	if (MaxDist <= KINDA_SMALL_NUMBER)
	{

		return;
	}

	UAnimMontage* Montage = Cast<UAnimMontage>(Animation);

	NotifyAnimCurrentTime = Anim->Montage_GetPosition(Montage);

	FVector CurrentBoneLoc = SampleBoneWorldLocation(MeshComp, Animation, NotifyAnimCurrentTime, WarpBoneName);
	float CurrentDist = FVector::Distance(CurrentBoneLoc, StartBoneLoc);
	float Scale = static_cast<float>(CurrentDist / MaxDist);
	//����׷��
	//TargetBoneLoc = EnemyAIRef->EnemyTargetActor->GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("UToggleBoneWarpNotifyState::NotifyTick: STARTING!!!"));
	if(TrackingTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UToggleBoneWarpNotifyState::NotifyTick: Tracking!!!"));
		TargetBoneLoc = EnemyAIRef->EnemyTargetActor->GetActorLocation();
		if (FMath::IsNearlyEqual(CurrentDist, MaxDist,5.f))
		{
			TrackingTarget = false;
		}
	}





	
	Scale = FMath::Clamp(Scale, 0, 1);
	//UE_LOG(LogTemp, Warning, TEXT("UToggleBoneWarpNotifyState::NotifyTick: StartBoneLoc : %s"), *StartBoneLoc.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("UToggleBoneWarpNotifyState::NotifyTick: MaxBoneLoc : %s"), *MaxBoneLoc.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("UToggleBoneWarpNotifyState::NotifyTick: CurrentBoneLoc : %s"), *CurrentBoneLoc.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("UToggleBoneWarpNotifyState::NotifyTick: TargetBoneLoc : %s"), *TargetBoneLoc.ToString());


	const FVector BoneWarpLoc = StartBoneLoc + Scale * (TargetBoneLoc - StartBoneLoc);
	const FRotator BoneWarpRot = MeshComp->GetSocketRotation(WarpBoneName);






	FAnimInstStruct AnimStruct;

	AnimStruct.BoneWarpWorldLoc = BoneWarpLoc;
	AnimStruct.BoneWarpWorldRot = BoneWarpRot;
	AnimStruct.BoneWarpActive = true;

	Anim->BoneWarpMap.FindOrAdd(WarpBoneName) = AnimStruct;

}

void UToggleBoneWarpNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp)
	{
		return;
	}

	if (UPlayerAnimInstance* Anim = Cast<UPlayerAnimInstance>(MeshComp->GetAnimInstance()))
	{
		if (FAnimInstStruct* Entry = Anim->BoneWarpMap.Find(WarpBoneName))
		{
			Entry->BoneWarpActive = false;
		}
	}

	EnemyAIRef = nullptr;
}


bool UToggleBoneWarpNotifyState::ResolveSequenceAtMontageTime(UAnimMontage* Montage, float MontageTime, UAnimSequence*& OutSequence, float& OutSequenceTime, int32 SlotIndex)
{
	OutSequence = nullptr;
	OutSequenceTime = 0.f;
	if (!Montage || !Montage->SlotAnimTracks.IsValidIndex(SlotIndex))
	{
		return false;
	}
	const FAnimTrack& Track = Montage->SlotAnimTracks[SlotIndex].AnimTrack;
	for (const FAnimSegment& Seg : Track.AnimSegments)
	{
		const float SegStart = Seg.StartPos;
		const float SegEnd = SegStart + Seg.GetLength();
		if (MontageTime + KINDA_SMALL_NUMBER < SegStart || MontageTime > SegEnd + KINDA_SMALL_NUMBER)
		{
			continue;
		}
		UAnimSequence* Seq = Cast<UAnimSequence>(Seg.GetAnimReference());
		if (!Seq)
		{
			return false;
		}
		// Montage ʱ��ӳ�䵽 Sequence ʱ�䣨�ٷ� FAnimSegment ���壩
		OutSequence = Seq;
		OutSequenceTime = Seg.AnimStartTime + (MontageTime - SegStart) * Seg.AnimPlayRate;
		OutSequenceTime = FMath::Clamp(OutSequenceTime, 0.f, Seq->GetPlayLength());
		return true;
	}
	return false;
}

FVector UToggleBoneWarpNotifyState::SampleBoneWorldLocation(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Anim, float TimeSeconds, FName BoneName)
{
		if (!Mesh || !Anim || BoneName.IsNone())
		{
			return FVector::ZeroVector;
		}

		UAnimInstance* AnimInst = Mesh->GetAnimInstance();
		if (!AnimInst)
		{
			return FVector::ZeroVector;
		}

		// ---------- 1) �õ��ɲ����� Sequence + ʱ�� ----------
		UAnimSequence* Sequence = Cast<UAnimSequence>(Anim);
		float SampleTime = TimeSeconds;
		if (UAnimMontage* Montage = Cast<UAnimMontage>(Anim))
		{
			if (!ResolveSequenceAtMontageTime(Montage, TimeSeconds, Sequence, SampleTime))
			{
				return FVector::ZeroVector;
			}
		}
		if (!Sequence)
		{
			// ���� Sequence / Montage���ٷ�����·������
			return FVector::ZeroVector;
		}
		SampleTime = FMath::Clamp(SampleTime, 0.f, Sequence->GetPlayLength());

		// ---------- 2) Bone ���� ----------
		const FBoneContainer& RequiredBones = AnimInst->GetRequiredBones();
		const int32 PoseBoneIndex = RequiredBones.GetPoseBoneIndexForBoneName(BoneName);
		if (PoseBoneIndex == INDEX_NONE)
		{
			return FVector::ZeroVector;
		}

		// ---------- 3) �ٷ���GetAnimationPose ----------
		FMemMark Mark(FMemStack::Get());
		FCompactPose Pose;
		FBlendedCurve Curve;
		UE::Anim::FStackAttributeContainer Attributes;
		Pose.SetBoneContainer(&RequiredBones);
		FAnimationPoseData PoseData(Pose, Curve, Attributes);
		// �ڶ����������Ƿ�� Root Motion��ֻҪ����һ�� false��
		FAnimExtractContext ExtractContext(static_cast<double>(SampleTime), /*bExtractRootMotion*/ false);
		Sequence->GetAnimationPose(PoseData, ExtractContext);
		// ---------- 4) Local �� �� Component Space ----------
		FCSPose<FCompactPose> ComponentPose;
		ComponentPose.InitPose(Pose);
		const FTransform BoneCS = ComponentPose.GetComponentSpaceTransform(
			FCompactPoseBoneIndex(PoseBoneIndex));
		// ---------- 5) Component Space �� World Space ----------
		return Mesh->GetComponentTransform().TransformPosition(BoneCS.GetLocation());
}

FVector UToggleBoneWarpNotifyState::SampleBoneWorldLocationFromPlayingMontage(USkeletalMeshComponent* Mesh, UAnimMontage* Montage, FName BoneName)
{
	if (!Mesh || !Montage)
	{
		return FVector::ZeroVector;
	}
	UAnimInstance* AnimInst = Mesh->GetAnimInstance();
	if (!AnimInst)
	{
		return FVector::ZeroVector;
	}
	const float MontageTime = AnimInst->Montage_GetPosition(Montage);
	return SampleBoneWorldLocation(Mesh, Montage, MontageTime, BoneName);
}



