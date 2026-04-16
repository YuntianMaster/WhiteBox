// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/PlayerCharacter.h"
#include "Engine/DamageEvents.h"


// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	ParticleMovementComp = FindComponentByClass<UProjectileMovementComponent>();
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileBase::HandleBeginOverlap(AActor* otherActor)
{
	if (!otherActor) return;
	APawn* PawnRef{Cast<APawn>(otherActor)};
	if (!PawnRef)return;
	if (!PawnRef->IsPlayerControlled()) return;
	FindComponentByClass<UParticleSystemComponent>()->SetTemplate(ParticleTemple);
	ParticleMovementComp->StopMovementImmediately();

	//CUSTOM DAMAGE
	//APlayerCharacter* PlayerRef{ Cast<APlayerCharacter>(otherActor) };
	//PlayerRef->StatsComp->ReduceHealth(FProjectDamage);


	FDamageEvent ProjectileAttackEvent{};

	PawnRef->TakeDamage(
		FProjectDamage,
		ProjectileAttackEvent,
		PawnRef->GetController(),
		this
	);


	USphereComponent* SphereRef{FindComponentByClass<USphereComponent>()};
	SphereRef->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FTimerHandle DeathTimeHandle{};
	GetWorldTimerManager().SetTimer(
		DeathTimeHandle,
		this,
		&AProjectileBase::DestoryProjectile,
		0.5f
	);
}

void AProjectileBase::DestoryProjectile()
{
	Destroy();
}

