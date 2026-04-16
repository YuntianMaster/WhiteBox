// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapon/GeneralProjectile.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"
#include "Props/PickUpActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interface/Damageable.h"
#include "Interface/ProjectileInteractable.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AGeneralProjectile::AGeneralProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));

	// 避免高速子弹穿透：启用子步进，提高模拟精度
	if (ProjectileMoveComp)
	{
		ProjectileMoveComp->bForceSubStepping = true;
		// 单步最大时间（越小精度越高，性能开销越大）
		ProjectileMoveComp->MaxSimulationTimeStep = 0.016f; // ~60 FPS
		// 每帧最多子步次数
		ProjectileMoveComp->MaxSimulationIterations = 8;
	}

}

// Called when the game starts or when spawned
void AGeneralProjectile::BeginPlay()
{
	Super::BeginPlay();
	Char = Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	NiagaraComp = GetComponentByClass<UNiagaraComponent>();
	if (NiagaraComp)
	{
		NiagaraComp->Deactivate();
	}

	// 确保投射物移动组件有 UpdatedComponent，用于带碰撞的 Sweep
	if (ProjectileMoveComp && !ProjectileMoveComp->UpdatedComponent)
	{
		ProjectileMoveComp->SetUpdatedComponent(GetRootComponent());
	}
	ProjectileMoveComp->Deactivate();

	UStaticMeshComponent* Mesh = GetComponentByClass<UStaticMeshComponent>();
	if (Mesh)
		Mesh->SetUseCCD(true);
}

// Called every frame
void AGeneralProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGeneralProjectile::DestoryProjectile() {
	UE_LOG(LogTemp, Warning, TEXT("DESTORY!"));
	Destroy();
}

void AGeneralProjectile::AttachProjectileToSocket()
{

	USkeletalMeshComponent* Mesh = Char->GetMesh();
	if (!Mesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Mesh is null!"));
		return;
	}
	FAttachmentTransformRules ArrowAttachRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true
	);
	GetRootComponent()->AttachToComponent(Mesh, ArrowAttachRules, "arrow");

	
}

void AGeneralProjectile::DetachProjectileToSocket()
{
	USkeletalMeshComponent* Mesh = Char->GetMesh();
	if (!Mesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Mesh is null!"));
		return;
	}
	FDetachmentTransformRules ArrowDetachRules(
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		true
	);
	GetRootComponent()->DetachFromComponent(ArrowDetachRules);
}

void AGeneralProjectile::Fire(float CharingTime)
{
	if (!Char)
	{
		UE_LOG(LogTemp, Warning, TEXT("AGeneralProjectile::Fire Char is null"));
		return;
	}

	UCameraComponent* CameraComp = Char->GetComponentByClass<UCameraComponent>();
	if (!CameraComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("AGeneralProjectile::Fire CameraComponent is null"));
		return;
	}

	FVector CameraDirection = CameraComp->GetForwardVector() * 10000;
	FVector CamerStart = CameraComp->GetComponentLocation();
	FVector CamerEnd = CamerStart + CameraDirection;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult HitResult;
	bool isHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		CamerStart,
		CamerEnd,
		TraceObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		2.f
	);
	FVector Direction = isHit ? HitResult.Location : CamerEnd;
	FVector ArrowDirection = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), Direction);
	ProjectileMoveComp->Activate();
	//DetachProjectileToSocket();
	float Gravity = UKismetMathLibrary::Lerp(MaxGravity, MinGravity, CharingTime);
	ProjectileMoveComp->ProjectileGravityScale = Gravity;
	float ProjectileSpeed = UKismetMathLibrary::Lerp(ProjectileMinSpeed, ProjectileMaxSpeed, CharingTime);
	ProjectileMoveComp->InitialSpeed = ProjectileSpeed;
	ProjectileMoveComp->MaxSpeed = ProjectileSpeed;
	ProjectileMoveComp->Velocity = ArrowDirection * ProjectileSpeed;
	if(NiagaraComp)
		NiagaraComp->Activate();
}

void AGeneralProjectile::OnHitHandle(AActor* HitObject)
{
	if (!HitObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnHitHandle called with null HitObject"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Hitted! %s"), *HitObject->GetName());
	if (HitObject->Tags.Contains("Player")|| HitObject->Tags.Contains("Arrow"))return;

	USceneComponent* HitRootComp = HitObject->GetRootComponent();
	if (!HitRootComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("HitObject has no valid root component: %s"), *HitObject->GetName());
		return;
	}





	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this); // 排除自身，避免扫到箭体
	FHitResult Hits;
	// 先扫 ProjectileDamageable (Channel7)，再扫 ProjectileInteractable (Channel8)，取最近的有效命中
	bool bSphereHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 90.f,
		15.f,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel7),
		false,
		ActorsToIgnore,
		bIsDebugMode ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		Hits,
		true
	);
	if (!bSphereHit)
	{
		bSphereHit = UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(),
			GetActorLocation(),
			GetActorLocation() + GetActorForwardVector() * 90.f,
			15.f,
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel8),
			false,
			ActorsToIgnore,
			bIsDebugMode ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
			Hits,
			true
		);
	}
	if (!bSphereHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("SphereTrace no hit"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SphereTrace hit: %s"), *Hits.BoneName.ToString());
		//判断是否可以互动
		if (HitObject->Implements<UProjectileInteractable>()) {

			FVector Start = Char->GetComponentByClass<UCameraComponent>()->GetComponentLocation();
			FVector End = HitObject->GetActorLocation();
			FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(Start, End);
			Cast<IProjectileInteractable>(HitObject)->TargetAddImpulse(Direction, ProjectilePower,Hits.BoneName);

			UKismetSystemLibrary::DrawDebugArrow(
				GetWorld(),
				Start,
				End,
				10,
				FLinearColor::Red,
				2,
				2
			);

		}

		if (HitObject->Implements<UDamageable>()) {

			UE_LOG(LogTemp, Warning, TEXT("UDamageable 11"));
			Cast<IDamageable>(HitObject)->TakeDamage(this,WeaponDamage);



		}
	}
	
	
	
	FHitResult CreateProjectileHit;
	bool bCreateProjectile;
	bCreateProjectile = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 90.f,
		15.f,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel9),
		false,
		ActorsToIgnore,
		bIsDebugMode ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		CreateProjectileHit,
		true
	);

	ProjectileMoveComp->Deactivate();
	NiagaraComp->Deactivate();
	FAttachmentTransformRules AttachRules(
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld,
		true

	);

	
	if (PickUpActor&&bCreateProjectile)
	{
		AActor* Arrow = GetWorld()->SpawnActor(PickUpActor, &GetActorTransform());
		Arrow->AttachToComponent(CreateProjectileHit.GetComponent(), AttachRules, CreateProjectileHit.BoneName);
		UE_LOG(LogTemp, Warning, TEXT("hit!!!!!!!!!!%s"),*CreateProjectileHit.GetComponent()->GetName());
	}
	


	
	Destroy();

	auto Responses = HitRootComp->GetCollisionResponseToChannels();
	if (Responses.GetResponse(ECC_GameTraceChannel5) == ECR_Block)
	{
		UE_LOG(LogTemp, Warning, TEXT("O!!!!!!"));
		if (HitObject->Implements<UDamageable>())
		{
			Cast<IDamageable>(HitObject)->TakeDamage(this,WeaponDamage);
			

		}
	}
}

float AGeneralProjectile::GetDamage()
{
	return WeaponDamage;
}

