// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/PlayerTraceComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"
#include "Interface/Fighter.h"
#include "Combat/WeaponSystemComp.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Combat/Weapon/WeaponGeneral.h"
#include "Enemy/DismembermentComponent.h"
#include "Structure/FWeaponStruct.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "GAS/GA/GA_WarpMontageAttack.h"
#include "Structure/FGASStucts.h"
#include "Kismet/KismetSystemLibrary.h"


static FName GetFirstAncestorBoneInPhysicsAsset(USkeletalMeshComponent* MeshComp, FName BoneName);
static uint32 MakeTraceKey(const USceneComponent* Comp, FName StartSocket, FName EndSocket);
// Sets default values for this component's properties
UPlayerTraceComponent::UPlayerTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerTraceComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnRef = GetOwner<ACharacter>();
	IFighter* FighterRef{ Cast<IFighter>(OwnRef) };
	WeaponSystemComp = OwnRef->GetComponentByClass<UWeaponSystemComp>();

}


// Called every frame
void UPlayerTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (!WeaponSystemComp) {

		return;
	}

	if (WeaponSystemComp->ECurrentWeapons == EWeapons::NoWeapon)
	{
		return;
	}
	if (!bIsAttacking) 
	{
		TargetsToIngore.Empty();
		PrevSocketPairs.Empty();
		return;
	}


	//UE_LOG(LogTemp, Warning, TEXT("Attacking!"));
	TArray<AWeaponGeneral*> FCurrentWeapons = 
		WeaponSystemComp->AllGaintedWeaponStructs[WeaponSystemComp->ECurrentWeapons];
	//TArray<FHitResult> AllOutHits;
	TMap<AActor*, FHitResult> AllOutHits;

	TArray<AActor*> ActorsToIgnore = TargetsToIngore;
	if (OwnRef)
	{
		ActorsToIgnore.AddUnique(OwnRef);
	}

	const FVector BoxHalfExtent = FVector(BoxCollisionLenght * 0.5f);
	const ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);
	const EDrawDebugTrace::Type DebugTraceType = bDebugMode ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	for (AWeaponGeneral* weapon : FCurrentWeapons) {
		if (!weapon || !weapon->WeaponMeshComp)
		{
			continue;
		}

		USkeletalMeshComponent* WeaponMesh = weapon->WeaponMeshComp;
		const FTraceSockets& WeaponTraceSockets = weapon->WeaponStructData.TraceSockets;

		FVector StartSockerLocation{ WeaponMesh->GetSocketLocation(WeaponTraceSockets.Start) };
		FVector EndtSockerLocation{ WeaponMesh->GetSocketLocation(WeaponTraceSockets.End) };
		FRotator ShapeRotaion{ WeaponMesh->GetSocketQuaternion(WeaponTraceSockets.Rotation) };

		TArray<FHitResult> OutHits;
		TArray<FHitResult> SweepOutHits;
		TArray<FHitResult> FinalSweeps;
		UKismetSystemLibrary::BoxTraceMulti(
			GetWorld(),
			StartSockerLocation,
			EndtSockerLocation,
			BoxHalfExtent,
			ShapeRotaion,
			TraceChannel,
			false,
			ActorsToIgnore,
			DebugTraceType,
			OutHits,
			true
		);


		if (bEnableSweptTrace) {
			const uint32 Key = MakeTraceKey(WeaponMesh, WeaponTraceSockets.Start, WeaponTraceSockets.End);
			FPrevSocketPair& Prev = PrevSocketPairs.FindOrAdd(Key);
			if (Prev.isVaild) {
				const FVector PrevStart = Prev.Start;
				const FVector PrevEnd = Prev.End;

				const FVector PreMid = (PrevStart + PrevEnd) * 0.5;
				const FVector CurMid = (StartSockerLocation + EndtSockerLocation) * 0.5;
				const float TraveDist = FVector::Distance(PreMid, CurMid);

				const int32 Steps = FMath::Clamp(
					FMath::CeilToInt(TraveDist/ FMath::Max(1.0f, SweptTraceStepDistance)),
					1,
					FMath::Max(1, SweptTraceMaxSubsteps)
				);

				const float Radius = FMath::Max(1, BoxCollisionLenght * 0.5f);
				for (size_t i = 0; i < Steps; ++i)
				{
					const float T0 = static_cast<float>(i) / static_cast<float>(Steps);
					const float T1 = static_cast<float>(i + 1) / static_cast<float>(Steps);

					const FVector S0 = FMath::Lerp(PrevStart, StartSockerLocation, T0);
					const FVector S1 = FMath::Lerp(PrevStart, StartSockerLocation, T1);

					const FVector E0 = FMath::Lerp(PrevEnd, EndtSockerLocation, T0);
					const FVector E1 = FMath::Lerp(PrevEnd, EndtSockerLocation, T1);

					const FVector M0 = (S0 + E0) * 0.5f;
					const FVector M1 = (S1 + E1) * 0.5f;

					UKismetSystemLibrary::SphereTraceMulti(
						GetWorld(),
						S0, S1,
						Radius,
						TraceChannel,
						false,
						ActorsToIgnore,
						DebugTraceType,
						SweepOutHits,
						true
					);


					FinalSweeps.Append(SweepOutHits);
					UKismetSystemLibrary::SphereTraceMulti(
						GetWorld(),
						M0, M1,
						Radius,
						TraceChannel,
						false,
						ActorsToIgnore,
						DebugTraceType,
						SweepOutHits,
						true
					);
					FinalSweeps.Append(SweepOutHits);
					UKismetSystemLibrary::SphereTraceMulti(
						GetWorld(),
						E0, E1,
						Radius,
						TraceChannel,
						false,
						ActorsToIgnore,
						DebugTraceType,
						SweepOutHits,
						true
					);
					FinalSweeps.Append(SweepOutHits);
				}
					
				for (const FHitResult& Hit : FinalSweeps) {

					AActor* Actor = Hit.GetActor();
					if (!Actor) continue;
					if (!AllOutHits.Contains(Actor) || Hit.Distance < AllOutHits[Actor].Distance)
						AllOutHits.Add(Actor, Hit);

				}

			}
			Prev.Start = StartSockerLocation;
			Prev.End = EndtSockerLocation;
			Prev.isVaild = true;

		}


		/*for (const FHitResult& Hit : OutHits) {

			AllOutHits.Add(Hit);
		}*/


	};

	
	UE_LOG(LogTemp, Warning, TEXT("Montage ID: %i"), Trace_MontageInstanceID);
	//UE_LOG(LogTemp, Warning, TEXT("AllOutHits: %i"), AllOutHits.Num());
//	UE_LOG(LogTemp, Warning, TEXT("GA_WarpMontageAttack is : %s"), *GA_WarpMontageAttack->GetName());
	if (AllOutHits.Num() == 0) return;
	

	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	float CharacterDamage{ 0.0f };
	IFighter* FighterRef = GetOwner<IFighter>();
	if(FighterRef)
		CharacterDamage = FighterRef->GetDamage();
	
	const FGameplayTag EventAttackHit = FGameplayTag::RequestGameplayTag(FName("Event.Combat.HitSuccess"));
	const FGameplayTag EventDefender = FGameplayTag::RequestGameplayTag(FName("Event.Combat.TakeHit"));
	const FGameplayTag EventTraceHit = FGameplayTag::RequestGameplayTag(FName("Event.Combat.TraceHitSucess"));

	FDamageEvent TargetAttackEvent;
	for (const TPair<AActor*, FHitResult>& Pair : AllOutHits) {

		AActor* TargetActor = Pair.Key;
		const FHitResult& Hit = Pair.Value;

		if (!IsValid(TargetActor))
		{
			continue;
		}

		FGameplayEventData Playload_TraceHit;
		Playload_TraceHit.EventTag = EventTraceHit;
		Playload_TraceHit.Instigator = OwnerActor;
		Playload_TraceHit.Target = TargetActor;
		FTraceMontageID* TraceMontageData = new FTraceMontageID();
		TraceMontageData->TRACE_MONTAGE_ID = Trace_MontageInstanceID;
		FGameplayAbilityTargetData_SingleTargetHit* const SingleTargetData = new FGameplayAbilityTargetData_SingleTargetHit();
		SingleTargetData->HitResult = Hit;
		Playload_TraceHit.TargetData.Add(TraceMontageData);
		Playload_TraceHit.TargetData.Add(SingleTargetData);


		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			OwnerActor,
			EventTraceHit,
			Playload_TraceHit
		);



		//FGameplayEventData Playload_Hit;
		//Playload_Hit.EventTag = EventAttackHit;
		//Playload_Hit.Instigator = OwnerActor;
		//Playload_Hit.Target = TargetActor;
		//Playload_Hit.OptionalObject2 = OwnerActor;

		//FGameplayAbilityTargetData_SingleTargetHit* const SingleTargetData = new FGameplayAbilityTargetData_SingleTargetHit();
		//SingleTargetData->HitResult = Hit;
	
		//FGameplayAbilityTargetDataHandle TargetHandle;
		//TargetHandle.Add(SingleTargetData);

		//Playload_Hit.TargetData = TargetHandle;

		//UE_LOG(LogTemp, Warning, TEXT("TargetActor: %s"), *TargetActor->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("Instigator: %s"), *OwnerActor->GetName());


	

		//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		//	OwnerActor,
		//	EventAttackHit,
		//	Playload_Hit);


		//FGameplayEventData Playload_Defender;
		//FGameplayAbilityTargetDataHandle DenfenderData;
		//Playload_Defender.EventTag = EventDefender;
		//Playload_Defender.Instigator = OwnerActor;
		//Playload_Defender.Target = TargetActor;
		//Playload_Defender.EventMagnitude = GA_WarpMontageAttack ? GA_WarpMontageAttack->DamageMagnitude : 0.0f;

		//FGameplayAbilityTargetData_SingleTargetHit* const SingleDefendData = new FGameplayAbilityTargetData_SingleTargetHit();
		//SingleDefendData->HitResult = Hit;
		//DenfenderData.Add(SingleDefendData);
		//Playload_Defender.TargetData = DenfenderData;

		//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		//	TargetActor,
		//	EventDefender,
		//	Playload_Defender);


		if (!IsValid(TargetActor) || TargetsToIngore.Contains(TargetActor)) continue;
		TargetActor->TakeDamage(
			CharacterDamage,
			TargetAttackEvent,
			OwnerActor->GetInstigatorController(),
			OwnerActor
		);

		UE_LOG(LogTemp, Warning, TEXT("ActorName: %s"), *TargetActor->GetName());
		FVector ImpactLoc = Hit.ImpactPoint;
		FName ImpactBone;
		ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor);
		USkeletalMeshComponent* TargetMesh = TargetCharacter ? TargetCharacter->GetMesh() : nullptr;
		if (TargetMesh)
		{
			ImpactBone = TargetMesh->FindClosestBone(ImpactLoc);
			UE_LOG(LogTemp, Warning, TEXT("BoneName: %s"),*ImpactBone.ToString());
		}
		//UE_LOG(LogTemp,Warning,TEXT("%f"), CharacterDamage);
		FRotator ImpactNor = UKismetMathLibrary::MakeRotFromZ(Hit.ImpactNormal);
		if (bloodParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				bloodParticle,
				Hit.ImpactPoint,
				ImpactNor
			);
		}
		//DamageDismemberment 

		UDismembermentComponent* DismemberComp = TargetActor->FindComponentByClass<UDismembermentComponent>();
		if (!DismemberComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Disme:"));
		}
		else {
			
			FName DismemberBoneName = GetFirstAncestorBoneInPhysicsAsset(TargetMesh, ImpactBone);
			if (!DismemberBoneName.IsNone())
			{
				DismemberComp->BodyTakeDamage(DismemberBoneName, CharacterDamage, Hit.ImpactNormal, Hit.ImpactPoint);
				UE_LOG(LogTemp, Warning, TEXT("Damage!!!!! %s"), *DismemberBoneName.ToString());
			}
		}

		TargetsToIngore.AddUnique(TargetActor);
	}

}

void UPlayerTraceComponent::HandleResestAttack()
{
	TargetsToIngore.Empty();
}

static FName GetFirstAncestorBoneInPhysicsAsset(USkeletalMeshComponent* MeshComp, FName BoneName)
{
	if (!MeshComp || BoneName.IsNone()) return NAME_None;
	UPhysicsAsset* PhysAsset = MeshComp->GetPhysicsAsset();
	USkeleton* Skeleton = MeshComp->GetSkeletalMeshAsset() ? MeshComp->GetSkeletalMeshAsset()->GetSkeleton() : nullptr;
	if (!PhysAsset || !Skeleton)
		return NAME_None;
	const FReferenceSkeleton& RefSkel = Skeleton->GetReferenceSkeleton();
	int32 BoneIndex = RefSkel.FindBoneIndex(BoneName);
	while (BoneIndex != INDEX_NONE)
	{
	
		FName CurrentBoneName = RefSkel.GetBoneName(BoneIndex);
		if (PhysAsset->FindBodyIndex(CurrentBoneName) != INDEX_NONE)
			return RefSkel.GetBoneName(BoneIndex);
		BoneIndex = RefSkel.GetParentIndex(BoneIndex);
	}
	return NAME_None;
}

static uint32 MakeTraceKey(const USceneComponent* Comp, FName StartSocket, FName EndSocket) {
	return HashCombine(HashCombine(GetTypeHash(Comp), GetTypeHash(StartSocket)), GetTypeHash(EndSocket));
}