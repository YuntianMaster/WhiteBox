// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapon/RockGeneral.h"
#include "Combat/Weapon/GeneralProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "kismet/KismetSystemLibrary.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SplineComponent.h"
#include "Structure/FProjecTilePoolActor.h"
#include "GameFramework/Character.h"
#include "Components/SplineMeshComponent.h"

ARockGeneral::ARockGeneral()
{
	WeaponName = EWeapons::GeneralRock;
	SplineComp = CreateDefaultSubobject<USplineComponent>("SplineComp");
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMeshComp;
	SplineComp->SetupAttachment(RootComponent);
}

void ARockGeneral::BeginPlay()
{
	Super::BeginPlay();
	for (size_t i = 0; i < PoolSize; i++)
	{
		FProjecTilePoolActor newActor;
		newActor.isActived = false;
		USplineMeshComponent* NewSplineMesh = NewObject<USplineMeshComponent>(this);
		
		NewSplineMesh->SetMobility(EComponentMobility::Movable);
		NewSplineMesh->RegisterComponent();
		NewSplineMesh->SetStaticMesh(TraceMesh);
		if (TraceMaterial)
		{
			const int32 NumSlots = TraceMesh ? TraceMesh->GetStaticMaterials().Num() : 0;
			const int32 ApplySlots = (NumSlots > 0) ? NumSlots : 1;
			for (int32 SlotIdx = 0; SlotIdx < ApplySlots; ++SlotIdx)
			{
				NewSplineMesh->SetMaterial(SlotIdx, TraceMaterial);
			}
		}
		NewSplineMesh->SetForwardAxis(ESplineMeshAxis::Z);
		NewSplineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		newActor.SplineMesh = NewSplineMesh;

		TracePool.Add(newActor);
		newActor.SplineMesh->SetHiddenInGame(true);

	}
}

void ARockGeneral::BlockFuction()
{
}

void ARockGeneral::ResertWeapon()
{
	SetActorHiddenInGame(true);
}

void ARockGeneral::ActiveWeapon()
{
	SetActorHiddenInGame(false);
	DrawTraceLine();
}

void ARockGeneral::Shooting()
{
	ChargingWeapon();

	GetWorld()->GetTimerManager().SetTimer(
		DrawTraceHandle,
		this,
		&ARockGeneral::DrawTraceLine,
		DrawFrequency,
		true
	);
}

void ARockGeneral::ReleaseCharing()
{
	AWeaponGeneral::ReleaseCharing();
	GetWorld()->GetTimerManager().ClearTimer(DrawTraceHandle);
	FTransform SocketTransform = PlayerRef->GetMesh()->GetSocketTransform(Weapons[0].HandSocketName);
	CreatedRockProjectile = GetWorld()->SpawnActor(RockProjectile, &SocketTransform);
	ActorsToIgnore.Add(CreatedRockProjectile);
	UStaticMeshComponent* Mesh = CreatedRockProjectile->GetComponentByClass<UStaticMeshComponent>();
	if (!Mesh) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Mesh is none"));
		return;
	}
	FVector CamDir = UKismetMathLibrary::GetForwardVector(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraRotation());
	CamDir.Z = UKismetMathLibrary::MapRangeUnclamped(CamDir.Z, 0, 1, PowerUpMin, PowerUpMax)*PowerOffset;
	Mesh->AddImpulse(CamDir*1000* charingTime, "None", true);
	//DrawTraceLine();
	//addSplineMesh();
}

void ARockGeneral::DrawTraceLine()
{
	SplineComp->ClearSplinePoints();
	FVector StartLoc = PlayerRef->GetMesh()->GetSocketLocation(Weapons[0].HandSocketName);

	FVector LaunchVelocity = UKismetMathLibrary::GetForwardVector(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraRotation());
	LaunchVelocity.Z = UKismetMathLibrary::MapRangeUnclamped(LaunchVelocity.Z, 0, 1, PowerUpMin, PowerUpMax) * PowerOffset;
	LaunchVelocity = LaunchVelocity * Power * charingTime;
	float ProjectileRadius = 5;
	float MaxSimTime = 12;
	ECollisionChannel TraceChannel = ECollisionChannel::ECC_WorldStatic;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	float PredictFrequency = 20;
	float GravityZ = 0;
	float DebugTime = 2;
	FPredictProjectilePathParams PredictParams;
	PredictParams.StartLocation = StartLoc;
	PredictParams.LaunchVelocity = LaunchVelocity;
	PredictParams.bTraceWithCollision = true;
	PredictParams.ProjectileRadius = ProjectileRadius;
	PredictParams.MaxSimTime = MaxSimTime;
	PredictParams.bTraceWithChannel = true;
	PredictParams.TraceChannel = TraceChannel;
	PredictParams.ActorsToIgnore = ActorsToIgnore;
	PredictParams.SimFrequency = PredictFrequency;
	PredictParams.OverrideGravityZ = GravityZ;
	PredictParams.DrawDebugType = EDrawDebugTrace::None;
	PredictParams.DrawDebugTime = DebugTime;
	PredictParams.bTraceComplex = false;
	FPredictProjectilePathResult PathResult;
	UGameplayStatics::PredictProjectilePath(GetWorld(), PredictParams, PathResult);
	PathResult.PathData;
	for (const FPredictProjectilePathPointData& points : PathResult.PathData)
	{
		SplineComp->AddSplinePoint(points.Location, ESplineCoordinateSpace::World, true);
	}
	addSplineMesh();
	//UE_LOG(LogTemp, Warning, TEXT("charingTime: %s"),*PathResult.HitResult.GetActor()->GetName());
}

void ARockGeneral::addSplineMesh()
{
	float LifeTime  = TraceLifeTime;
	for (size_t i = 1; i < SplineComp->GetNumberOfSplinePoints() - 1; i++)
	{
		
		FProjecTilePoolActor* PoolActor = GetFirstUnActivedActor();
		if (!PoolActor) return;


		FVector StartLocation;
		FVector StartTangent;
		SplineComp->GetLocationAndTangentAtSplinePoint(i, StartLocation, StartTangent, ESplineCoordinateSpace::World);
		FVector EndLocation;
		FVector EndTangent;
		SplineComp->GetLocationAndTangentAtSplinePoint(i + 1, EndLocation, EndTangent, ESplineCoordinateSpace::World);


		EndLocation = UKismetMathLibrary::VLerp(StartLocation, EndLocation, 0.5);
		EndTangent = UKismetMathLibrary::VLerp(StartTangent, EndTangent, 0.5);
		PoolActor->SplineMesh->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent);

		PoolActor->SplineMesh->SetHiddenInGame(false);
		PoolActor->isActived = true;
		LifeTime += LastFrequcy;
		FTimerHandle DelayResetPoolActor;

		GetWorld()->GetTimerManager().SetTimer(
			DelayResetPoolActor,
			[this, PoolActor]() {
				ResetProjectileHandle(PoolActor);
			},
			LifeTime,
			false

		);
	}
}



FProjecTilePoolActor* ARockGeneral::GetFirstUnActivedActor()
{

	for (FProjecTilePoolActor& PoolActor : TracePool) {
		if (!PoolActor.isActived)
			return &PoolActor;
	}
	UE_LOG(LogTemp, Warning, TEXT("No Capacity!!"))
	return nullptr;
}


void ARockGeneral::ResetProjectileHandle(FProjecTilePoolActor* ProjectActor)
{
	ProjectActor->isActived = false;
	ProjectActor->SplineMesh->SetHiddenInGame(true);
}

