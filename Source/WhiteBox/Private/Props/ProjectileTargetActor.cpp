// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/ProjectileTargetActor.h"
#include "Components/WidgetComponent.h"
#include "Widget/DamagePropsWidget.h"
#include "Player/StatsComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

// Sets default values
AProjectileTargetActor::AProjectileTargetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));
	SkeletlMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletonMeshComp"));
	SkeletlMeshComp->SetupAttachment(RootComponent);

	UserWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	UserWidgetComp->SetupAttachment(RootComponent);

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComp"));
	

	

}

// Called when the game starts or when spawned
void AProjectileTargetActor::BeginPlay()
{
	Super::BeginPlay();
	//CreateWidget(GetWorld(),TargetWidgetClass)->AddToViewport();
	if(!TargetWidgetClass)
	{
		return;
	}
	if(!UserWidgetComp)
	{
		UE_LOG(LogTemp, Error, TEXT("AProjectileTargetActor: UserWidgetComp is null (Check Blueprint components: WidgetComp)"));
		return;
	}
	UserWidgetComp->SetWidgetClass(TargetWidgetClass);
	UserWidgetComp->SetWidgetSpace(EWidgetSpace::World);
	// 确保 WidgetClass 已经实例化，避免 GetUserWidgetObject() 返回 nullptr
	UserWidgetComp->InitWidget();
	DamagePropsObject = Cast<UDamagePropsWidget>(UserWidgetComp->GetUserWidgetObject());
	if(!DamagePropsObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("AProjectileTargetActor: DamagePropsObject cast failed (TargetWidgetClass=%s)"), *TargetWidgetClass->GetName());
	}
	DamagePropsObject->SetPercertage(StatsComp->GetStatPercetage(EStats::Health, EStats::MaxHealth));
	UserWidgetComp->SetHiddenInGame(true);
}

// Called every frame
void AProjectileTargetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (UserWidgetComp)
	{
		FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
		FVector UILocation = UserWidgetComp->GetComponentLocation();
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(UILocation, CameraLocation);
		LookAtRot.Pitch = 0.f;
		LookAtRot.Roll = 0.f;
		UserWidgetComp->SetWorldRotation(LookAtRot);
	
	}



}

void AProjectileTargetActor::TargetAddImpulse(FVector ImpulseVector, float Power , FName BoneName)
{
	IProjectileInteractable::TargetAddImpulse(ImpulseVector,Power,BoneName);
	if(!SkeletlMeshComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("AProjectileTargetActor: SkeletlMeshComp is null"));
		return;
	}
	int32 BoneTarget = SkeletlMeshComp->GetBoneIndex(BoneName);
	if(BoneTarget != INDEX_NONE )
		SkeletlMeshComp->AddImpulse(ImpulseVector * Power, BoneName, false);
}

void AProjectileTargetActor::TakeDamage(AActor* Causer,float Damage)
{
	if (!DamagePropsObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("No DamagaUI"));
		return;
	}
	if(!StatsComp)
	{
		UE_LOG(LogTemp, Error, TEXT("AProjectileTargetActor: StatsComp is null"));
		return;
	}
	FDamageEvent TargetAttackEvent;
	StatsComp->ReduceHealth(Damage, Causer);
	DamagePropsObject->SetPercertage(StatsComp->GetStatPercetage(EStats::Health, EStats::MaxHealth));
	UserWidgetComp->SetHiddenInGame(false);
	FLatentActionInfo ActionInfo(
		0,
		700,
		TEXT("HideUI"),
		this
	
	);
	UKismetSystemLibrary::RetriggerableDelay(GetWorld(), existTime, ActionInfo);

}

float AProjectileTargetActor::OnHandleDeath()
{
	return 0.5f;
}

void AProjectileTargetActor::HideUI()
{
	if (UserWidgetComp) {

		UserWidgetComp->SetHiddenInGame(true);

	}
}



