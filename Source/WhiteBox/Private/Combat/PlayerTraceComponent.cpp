// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/PlayerTraceComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"
#include "Interface/Fighter.h"
#include "Combat/WeaponSystemComp.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/DismembermentComponent.h"
#include "Structure/FWeaponStruct.h"
#include "PhysicsEngine/PhysicsAsset.h"
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

	if (!WeaponSystemComp->CurrentWeapon)
	{
		return;
	}
	if (!bIsAttacking) 
	{
		TargetsToIngore.Empty();
		PrevSocketPairs.Empty();
		return;
	}
	TArray<FHitResult> AllOutHits;
	for (const FWeaponStruct& weapon : WeaponSystemComp->CurrentWeapon->Weapons) {
		 

		FVector StartSockerLocation{ weapon.WeaponComp->GetSocketLocation(weapon.TraceSockets.Start) };
		FVector EndtSockerLocation{ weapon.WeaponComp->GetSocketLocation(weapon.TraceSockets.End) };
		FRotator ShapeRotaion{ weapon.WeaponComp->GetSocketQuaternion(weapon.TraceSockets.Rotation) };

		TArray<FHitResult> OutHits;
		TArray<FHitResult> SweepOutHits;
		FCollisionQueryParams IngoreParams{ FName{"Igorna"}, false, GetOwner() };

		double WeaponDistance{
			FVector::Distance(StartSockerLocation,EndtSockerLocation)
		};
		TArray<AActor*> ActorsToIgnore = TargetsToIngore;
		if (OwnRef)
		{
			ActorsToIgnore.AddUnique(OwnRef);   
		}
		FVector BoxHalfExtent{ BoxCollisionLenght ,BoxCollisionLenght ,BoxCollisionLenght };
		BoxHalfExtent /= 2;
		FCollisionShape box{ FCollisionShape::MakeBox(BoxHalfExtent) };
		bool bHasFoundTarget = UKismetSystemLibrary::BoxTraceMulti(
			GetWorld(),
			StartSockerLocation,
			EndtSockerLocation,
			BoxHalfExtent,
			ShapeRotaion,
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
			false,
			ActorsToIgnore,
			bDebugMode?EDrawDebugTrace::ForDuration:EDrawDebugTrace::None,
			OutHits,
			true
		
		);


		if (bEnableSweptTrace && weapon.WeaponComp) {
			const uint32 Key = MakeTraceKey(weapon.WeaponComp, weapon.TraceSockets.Start, weapon.TraceSockets.End);
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
						UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
						false,
						ActorsToIgnore,
						bDebugMode ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
						SweepOutHits,
						true
					);

					UKismetSystemLibrary::SphereTraceMulti(
						GetWorld(),
						M0, M1,
						Radius,
						UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
						false,
						ActorsToIgnore,
						bDebugMode ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
						SweepOutHits,
						true
					);

					UKismetSystemLibrary::SphereTraceMulti(
						GetWorld(),
						E0, E1,
						Radius,
						UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
						false,
						ActorsToIgnore,
						bDebugMode ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
						SweepOutHits,
						true
					);
				}
					
				for (const FHitResult& Hit : SweepOutHits) {

					AllOutHits.Add(Hit);
				}

			}
			Prev.Start = StartSockerLocation;
			Prev.End = EndtSockerLocation;
			Prev.isVaild = true;

		}


		for (const FHitResult& Hit : OutHits) {

			AllOutHits.Add(Hit);
		}


	};

	

	if (AllOutHits.Num() == 0) return;

	float CharacterDamage{ 0.0f };
	IFighter* FighterRef = GetOwner<IFighter>();
	if(FighterRef)
		CharacterDamage = FighterRef->GetDamage();

	FDamageEvent TargetAttackEvent;
	for (const FHitResult& Hit : AllOutHits) {
		AActor* TargetActor{ Hit.GetActor() };
		 
		if (TargetsToIngore.Contains(TargetActor)) continue;
		TargetActor->TakeDamage(
			CharacterDamage,
			TargetAttackEvent,
			GetOwner()->GetInstigatorController(),
			GetOwner()
		);

		UE_LOG(LogTemp, Warning, TEXT("ActorName: %s"), *TargetActor->GetName());
		FVector ImpactLoc = Hit.ImpactPoint;
		FName ImpactBone;
		ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor);
		if (TargetCharacter)
		{
			ImpactBone = TargetCharacter->GetMesh()->FindClosestBone(ImpactLoc);
			UE_LOG(LogTemp, Warning, TEXT("BoneName: %s"),*ImpactBone.ToString());
		}
		//UE_LOG(LogTemp,Warning,TEXT("%f"), CharacterDamage);
		FRotator ImpactNor = UKismetMathLibrary::MakeRotFromZ(Hit.ImpactNormal);
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			bloodParticle,
			Hit.ImpactPoint,
			ImpactNor
		);
		//DamageDismemberment 

		if (!TargetActor->GetComponentByClass<UDismembermentComponent>())
		{
			UE_LOG(LogTemp, Warning, TEXT("No Disme:"));
		}
		else {
			
			FName DismemberBoneName = GetFirstAncestorBoneInPhysicsAsset(TargetActor->GetComponentByClass<USkeletalMeshComponent>(), ImpactBone);
			if (!DismemberBoneName.IsNone())
			{
				TargetActor->GetComponentByClass<UDismembermentComponent>()->BodyTakeDamage(DismemberBoneName, CharacterDamage, Hit.ImpactNormal,Hit.ImpactPoint);
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