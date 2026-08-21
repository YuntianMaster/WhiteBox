// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapon/GeneralProjectile_AI.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy/EnemyAIController.h"
#include "Enemy/EnemyCharacter.h"

void AGeneralProjectile_AI::Fire(float CharingTime)
{


	OwnerActor = GetInstigator();
	EnemyTargetActor = Cast<AEnemyAIController>(Cast<AEnemyCharacter>(OwnerActor)->GetController())->EnemyTargetActor;
	
	auto RandomTargetLocation = [&](AActor* Target, AActor* OwnerC)->FVector {

		FVector TargetLocation = Target->GetActorLocation();
		FVector OwnerLocation = OwnerC->GetActorLocation();
		FVector Forward = (TargetLocation - OwnerLocation).GetSafeNormal();
		FVector Side = FVector::CrossProduct(FVector::UpVector, Forward).GetSafeNormal();
		// 2) 若几乎竖直瞄准，Cross 会接近 0，换参考轴
		if (Side.IsNearlyZero())
		{
			Side = FVector::CrossProduct(FVector::RightVector, Forward).GetSafeNormal();
		}

		FVector Up = FVector::CrossProduct(Forward, Side).GetSafeNormal();

		float SideOffset = UKismetMathLibrary::RandomFloatInRange(MinOffset, MaxOffset);
		float UpOffset = UKismetMathLibrary::RandomFloatInRange(MinOffset, MaxOffset);
		float Distance = FVector::Distance(OwnerLocation, TargetLocation) * 0.5f;
		SideOffset = bIsRight ? SideOffset : SideOffset * -1;
		UpOffset = bIsUp ? UpOffset : UpOffset * -1;
		return FMath::Lerp(OwnerLocation, TargetLocation, 0.5f) + SideOffset * Side * Distance + UpOffset * Up * Distance;
		};

	if (!EnemyTargetActor)
	{

		UE_LOG(LogTemp, Warning, TEXT("No TargetActor!!"));
		return;
	}
	BezierLocation = RandomTargetLocation(EnemyTargetActor, OwnerActor);
	ProjectileMoveComp->Activate();
}


void AGeneralProjectile_AI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ProjectileMoveComp->IsActive()) {

		SetBezierVelocity();
	}
	
}

void AGeneralProjectile_AI::SetBezierVelocity()
{
	ProjectileMoveTime += GetWorld()->DeltaTimeSeconds;
	float Alpha = FMath::Clamp(ProjectileMoveTime / CurveDuration, 0.f, 1.f);
	FVector CurrentBezierLocation = FMath::Lerp(BezierLocation, EnemyTargetActor->GetActorLocation(), Alpha);

	ProjectileMoveComp->Velocity =  UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentBezierLocation).Vector()*ProjectileSpeed;
}
