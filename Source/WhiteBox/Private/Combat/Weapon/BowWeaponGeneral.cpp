// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapon/BowWeaponGeneral.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/PlayerActionsComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Combat/Weapon/GeneralProjectile.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/AimingIconWidget.h"
#include "Player/PlayerCharacter.h"

ABowWeaponGeneral::ABowWeaponGeneral()
{
	WeaponName = EWeapons::GeneralBow;
}

void ABowWeaponGeneral::Tick(float DeltaTime)
{
	
}
//Aiming
void ABowWeaponGeneral::BlockFuction()
{
	if (!CharRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharRef is null!"));
		return;
	}

	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimInstance is null!"));
		return;
	}

	if (!BlockMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("BlockMontage is null!"));
		return;
	}


	GetWorld()->GetTimerManager().ClearTimer(FAimToNormalHandle);
	GetWorld()->GetTimerManager().SetTimer(
		FNormalToAimHandle,
		this,
		&ABowWeaponGeneral::CamerFollowToAim,
		GetWorld()->DeltaTimeSeconds,
		true
	);
	AnimInstance->bIsBowAiming = true;
	BowStats = EBowStats::BowAim;
	CharRef->PlayAnimMontage(BlockMontage);
	//Make Sure Arrow Created
	AnimInstance->Montage_SetEndDelegate(FAimingDelegate, BlockMontage);
	MovementRef->MaxWalkSpeed = AimWalkSpeed;
	ActionRef->SprintSpeed = AimSprintSpeed;
	if(!AimWidget->IsInViewport())
		AimWidget->AddToViewport();

}

void ABowWeaponGeneral::CancelBlockFuction()
{
	GetWorld()->GetTimerManager().ClearTimer(FNormalToAimHandle);
	CharRef->GetMesh()->GetAnimInstance()->Montage_Stop(0.01f, BlockMontage);
	GetWorld()->GetTimerManager().SetTimer(
		FAimToNormalHandle,
		this,
		&ABowWeaponGeneral::CamerAimToFollow,
		GetWorld()->DeltaTimeSeconds,
		true
	);
	BowStats = EBowStats::BowIdle;
	MovementRef->MaxWalkSpeed = NormalWalkSpeed;
	ActionRef->SprintSpeed = NormalSprintSpeed;
	//删除箭矢
	if (CurrentProjectile)
	{
		CurrentProjectile->SetActorHiddenInGame(true);
	}
	Super::ReleaseCharing();
	CharRef->GetController()->SetIgnoreMoveInput(false);
	AnimInstance->bIsBowAiming = false;
	AimWidget->RemoveFromParent();
	
}

void ABowWeaponGeneral::ReleaseCharing()
{
	
	Super::ReleaseCharing();
	if (!isDrawing) {
		return;
	}
	isDrawing = false;
	if (!AnimInstance->bIsBowAiming) return;
	CharRef->GetController()->SetIgnoreMoveInput(false);
	BowStats = EBowStats::BowShoot;	
	if (!Arrow)
	{
		UE_LOG(LogTemp, Warning, TEXT("Arrow class is null!"));
		return;
	}
	ACharacter* Char = Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	AGeneralProjectile* NewArrow = CreateArrow();
	//重新进入Aiming
	//BowStats = EBowStats::BowIdle;
	BlockFuction();
	//如果没精力，退出Aiming
	FReleaseCharingDelegate.Broadcast();
	NewArrow->Fire(charingTime / MaxCharingTime);
	UGameplayStatics::SpawnSoundAtLocation(
		GetWorld(),
		BowShoot,
		GetActorLocation()
	);
	AimWidget->OnShootingDelegate.Broadcast();


}
//Shooting charing
void ABowWeaponGeneral::Shooting()
{
	if (BowStats == EBowStats::BowAim)
	{
		if (!bIsReadyToFire)
			return;
		isDrawing = true;
		CharRef->GetController()->SetIgnoreMoveInput(true);
		AnimInstance->Montage_Play(DrawBowMontage, 1.f, EMontagePlayReturnType::MontageLength, 0.f, true);
		BowStats= EBowStats::BowDraw;
		ChargingWeapon();
		UGameplayStatics::SpawnSoundAtLocation(
			GetWorld(),
			BowDraw,
			GetActorLocation()
		);
		bIsReadyToFire = false;
		AimWidget->OnAimingDelegate.Broadcast();
	}
}

float ABowWeaponGeneral::WeaponAttack(UAnimMontage* AttackMontage)
{
	if (BowStats == EBowStats::BowAim)
		return 0.0f;
	else
		return Super::WeaponAttack(AttackMontage);
}

void ABowWeaponGeneral::ResertWeapon()
{
	CancelBlockFuction();
}

void ABowWeaponGeneral::BeginPlay()
{
	Super::BeginPlay();

	if (!GetWorld()) return;
	if(!GetWorld()->GetFirstPlayerController()->GetPawn())return;
	CharRef = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	//MovementIntial
	MovementRef = CharRef->GetCharacterMovement();
	ActionRef = CharRef->ActionComp;
	NormalSprintSpeed = ActionRef->SprintSpeed;
	NormalWalkSpeed = MovementRef->MaxWalkSpeed;


	//Camera Initial
	CameraRef = CharRef->GetComponentByClass<UCameraComponent>();
	CamearBoom = CharRef->GetComponentByClass<USpringArmComponent>();
	initialFOV = CameraRef->FieldOfView;
	initialOffset = CamearBoom->SocketOffset;


	//AimingCheckArrow

	FAimingDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted) {
		if (BowStats == EBowStats::BowAim)
		{
			UE_LOG(LogTemp, Warning, TEXT("TryAim"));
			if (!CurrentProjectile)
			{
				UE_LOG(LogTemp, Warning, TEXT("CreateArrow"));
				GetArrow();
			}
		}

		if (!bInterrupted)
			bIsReadyToFire = true;
		
		});

	//Aiming Icon
	AimWidget = Cast<UAimingIconWidget>(CreateWidget(GetWorld(), AimIcon));
}

void ABowWeaponGeneral::CamerFollowToAim()
{
	//Move Camera
	if (FMath::IsNearlyEqual(CameraRef->FieldOfView, AimFOV, 5.f))
		GetWorld()->GetTimerManager().ClearTimer(FNormalToAimHandle);
	CameraRef->FieldOfView = UKismetMathLibrary::FInterpTo_Constant(CameraRef->FieldOfView, AimFOV, GetWorld()->DeltaTimeSeconds, CameraMoveSpeed);
	float percent =  (CameraRef->FieldOfView - initialFOV) / (AimFOV - initialFOV);
	CamearBoom->SocketOffset = UKismetMathLibrary::VLerp(CamearBoom->SocketOffset, AimOffset, percent);	
}

void ABowWeaponGeneral::CamerAimToFollow()
{
	if (FMath::IsNearlyEqual(CameraRef->FieldOfView, initialFOV, 5.f))
		GetWorld()->GetTimerManager().ClearTimer(FAimToNormalHandle);
	CameraRef->FieldOfView = UKismetMathLibrary::FInterpTo_Constant(CameraRef->FieldOfView, initialFOV, GetWorld()->DeltaTimeSeconds, CameraMoveSpeed);
	float percent = (CameraRef->FieldOfView - AimFOV) / (initialFOV - AimFOV);
	CamearBoom->SocketOffset = UKismetMathLibrary::VLerp(CamearBoom->SocketOffset, initialOffset, percent);



}

AGeneralProjectile* ABowWeaponGeneral::CreateArrow()
{
	if (!Arrow)
	{
		UE_LOG(LogTemp, Warning, TEXT("Arrow class is null!"));
		return nullptr;
	}
	ACharacter* Char = Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	//if (ArrowProjectile) return;
	return Cast<AGeneralProjectile>(GetWorld()->SpawnActor(Arrow, &CurrentProjectile->GetActorTransform()));
 }

void ABowWeaponGeneral::GetArrow()
{
	if (!PlayerArrow)
	{
		UE_LOG(LogTemp, Warning, TEXT("Arrow class is null!"));
		return;
	}
	ACharacter* Char = Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if(!CurrentProjectile)
		CurrentProjectile = GetWorld()->SpawnActor(PlayerArrow, &Char->GetActorTransform());
	
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
	CurrentProjectile->GetRootComponent()->AttachToComponent(Mesh, ArrowAttachRules, "arrow");
	CurrentProjectile->SetActorHiddenInGame(false);
}


