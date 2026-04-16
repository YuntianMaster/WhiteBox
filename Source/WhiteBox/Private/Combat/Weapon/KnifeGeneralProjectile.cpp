// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapon/KnifeGeneralProjectile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Combat/WeaponSystemComp.h"
#include "Enemy/WeaknessActor.h"
#include "Combat/Weapon/WeaponGeneral.h"

void AKnifeGeneralProjectile::Fire(float CharingTime)
{
	if(NiagaraComp)
		NiagaraComp->Activate();
	if (!bIsRandomPath||!ProjectileTarget) {
			Super::Fire(CharingTime);
			return;
	}

	else {
		
		SpawnRandomloc = RandomTargetLocation(ProjectileTarget);
		/*FRotator spawnRot = FRotator::ZeroRotator;
		GetWorld()->SpawnActor(DebugActor, &spawnloc, &spawnRot);*/
		bIsRandomPathMove = true;
		
	}
	}	 

FVector AKnifeGeneralProjectile::RandomTargetLocation(AActor* Target)
{
	FVector Playerloc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector Targetloc = Target->GetActorLocation();
	FVector dir = Targetloc - Playerloc;

	
	dir = Targetloc + dir * UKismetMathLibrary::RandomFloatInRange(minDistance, maxDistance);
	//UKismetSystemLibrary::DrawDebugLine(GetWorld(), Targetloc, dir, FLinearColor::Blue, 10.f, 5.f);
	FVector DebugStart = dir;
	FVector TargetToDir = dir - Targetloc;

	float Dist = TargetToDir.Size();
	FVector Direction = TargetToDir.GetSafeNormal();
	float randomAngle = UKismetMathLibrary::RandomFloatInRange(minOffset, maxOffset);

	FVector RandDir = FMath::VRandCone(
		Direction,
		FMath::DegreesToRadians(randomAngle)
	);

	dir = Targetloc + RandDir * Dist;
	//UKismetSystemLibrary::DrawDebugLine(GetWorld(), Targetloc, dir, FLinearColor::Green, 10.f, 5.f);
	return dir;
}

void AKnifeGeneralProjectile::Tick(float DeltaTime)
{
	if (bIsRandomPathMove)
		StartRandomPath();
	if (ChecktheWeaknessHitHandle())
	{
		if(OutHit.GetActor() == ProjectileTarget)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *OutHit.GetActor()->GetName());
			OnHitHandle(ProjectileTarget);
		}
	}

}

void AKnifeGeneralProjectile::SetTarget(AActor* Target)
{
	ProjectileTarget = Target;
}

void AKnifeGeneralProjectile::StartRandomPath()
{
	SpawnRandomloc = UKismetMathLibrary::VInterpTo_Constant(SpawnRandomloc, ProjectileTarget->GetActorLocation(), GetWorld()->DeltaTimeSeconds, ProjectileSpeed);
	SetActorLocation(UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(), SpawnRandomloc, GetWorld()->DeltaTimeSeconds, ProjectileSpeed));
}

bool AKnifeGeneralProjectile::ChecktheWeaknessHitHandle()
{
	FVector StartLoc = GetActorLocation();
	FVector EndLoc = StartLoc;
	EndLoc.X = EndLoc.X + 50;
	TArray<TEnumAsByte<EObjectTypeQuery>> CheckObject;
	CheckObject.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
	TArray<AActor*> IngoreActor;
	bool HitResult =  UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc,
		CheckRadiu,
		CheckObject,
		false,
		IngoreActor,
		bIsDebugMode ? EDrawDebugTrace::ForDuration:EDrawDebugTrace::None,
		OutHit,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.f
	);
	return HitResult;
}
