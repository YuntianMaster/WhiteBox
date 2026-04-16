// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TravelingComp.h"
#include "Structure/FRopeSockets.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Travel/RopeActor.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Player/UnlockActionComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UTravelingComp::UTravelingComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTravelingComp::BeginPlay()
{
	Super::BeginPlay();
	SkeletalComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	PlayerControlRef = GetOwner()->GetInstigatorController<APlayerController>();
	Movementcomp = GetOwner<ACharacter>()->GetCharacterMovement();
	MontageEndDelegate.BindUFunction(this, "MontageEndHandler");

	LockActionComp = GetOwner()->FindComponentByClass<UUnlockActionComponent>();
	// ...
	
}


// Called every frame
void UTravelingComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	// ...
}

void UTravelingComp::TryGetRopeStartLoc()
{

	if (CheckStartPoint())
	{
		RopeRef = Cast<ARopeActor>(OutHit.GetActor());
		if (!RopeRef) return;
		ACharacter* CharacterRef{ GetOwner<ACharacter>() };
		if (!CharacterRef) return;

		//位移至开始点
		GetOwner()->SetActorLocation(
			FVector{
				RopeRef->StartLoc.X, 
				RopeRef->StartLoc.Y, 
				RopeRef->StartLoc.Z - 100.f
			},false,nullptr,ETeleportType::None
			);
		float DurationClimb = GetOwner<ACharacter>()->PlayAnimMontage(RopeClimb);
		FTimerHandle HandleClimb;

		StartMove(RopeRef->EndLoc, DurationClimb);


		

		if (!Movementcomp) return;
		Movementcomp->SetMovementMode(MOVE_None);
		//摄像机追踪角色
		GetOwner()->GetWorldTimerManager().SetTimer(TurnHandle, this, &UTravelingComp::TurnCameraBehind, 0.01f, true);


		if (PlayerControlRef)
		{
			//GetOwner()->DisableInput(PlayerControlRef);
		}
		//防止摄像机穿入
		USpringArmComponent* SpringArm = CharacterRef->FindComponentByClass<USpringArmComponent>();
		if (SpringArm)
		{
			SpringArm->bDoCollisionTest = false;
			FRotator Rot = SpringArm->GetRelativeRotation();
			SpringArm->SetRelativeRotation(Rot);
		}
		GetOwner()->SetActorRotation(RopeRef->RopeRot);
		CharacterRef->UpdateComponentTransforms();


		bIsRopeClimbing = true;

	}
	
}

void UTravelingComp::TurnCameraBehind()
{
	if (!PlayerControlRef) return;

	const float Delta = GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.01f;
	const float Speed = 6.f;
	FRotator Current = PlayerControlRef->GetControlRotation().GetNormalized();
	FRotator Target = GetOwner()->GetActorRotation().GetNormalized();
	FRotator NewRot = FMath::RInterpTo(Current, Target, Delta, Speed).GetNormalized();

	PlayerControlRef->SetControlRotation(NewRot);
	if (NewRot.Equals(Target, 0.1f)) {

		GetOwner()->GetWorldTimerManager().ClearTimer(TurnHandle);

	}

}

void UTravelingComp::MovetoLocation(FVector Endlocation)
{
	
	FVector StartLoc{ GetOwner()->GetActorLocation()};
	UE_LOG(LogTemp, Warning, TEXT("MOVING3"));
	const float Delta = GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.01f;
	const float Speed = 3.f;
	FVector NewLoc = FMath::VInterpTo(StartLoc, Endlocation, Delta, Speed);
	GetOwner()->SetActorLocation(NewLoc);
	if (NewLoc.Equals(Endlocation, 25.0f)) {
		GetOwner()->GetWorldTimerManager().ClearTimer(MoveHandle);
		Movementcomp->SetMovementMode(MOVE_Falling);
		GetOwner<ACharacter>()->PlayAnimMontage(LandingMontage);
		bIsRopeClimbing = false;
		UE_LOG(LogTemp, Warning, TEXT("START2,%s"), *Endlocation.ToString());
		GetOwner()->EnableInput(PlayerControlRef);

		USpringArmComponent* SpringArm = GetOwner<ACharacter>()->FindComponentByClass<USpringArmComponent>();
		if (SpringArm)
			SpringArm->bDoCollisionTest = true;
	}



}

void UTravelingComp::StartMove(FVector Endlocation,float Duration)
{
	FTimerHandle F_StartMoveHandle;
	GetOwner()->GetWorldTimerManager().SetTimer(
		F_StartMoveHandle,
		FTimerDelegate::CreateUObject(this,&UTravelingComp::StartMoveHandle, Endlocation),
		Duration,
		false
	);
}

void UTravelingComp::StartMoveHandle(FVector Endlocation)
{
	
	GetOwner()->GetWorldTimerManager().SetTimer(
		MoveHandle,
		FTimerDelegate::CreateUObject(this, &UTravelingComp::MovetoLocation, Endlocation),
		0.01f,
		true
	);
}

bool UTravelingComp::CheckStartPoint()
{

	FVector RopeSocketStart = SkeletalComp->GetSocketLocation(RopeSocket.Start);
	FVector RopeSocketEnd = SkeletalComp->GetSocketLocation(RopeSocket.End);
	RopeSocketEnd += FVector{ 0.f,0.f , DistanceCheck };
	FRotator RopeLookAtRot = FRotator::ZeroRotator;
	FQuat RopeQuat = FQuat::Identity;
	FCollisionQueryParams IngoreParams{ FName{"Igorna"},false,GetOwner() };
	double CheckDistance{
		FVector::Distance(RopeSocketStart,RopeSocketEnd)
	};

	FVector BoxHalfExtent{ CheckSize,CheckSize,CheckDistance };
	BoxHalfExtent /= 2;
	FCollisionShape box{ FCollisionShape::MakeBox(BoxHalfExtent) };


	//检查Rope开始点
	bool isRopePoint{ GetWorld()->SweepSingleByChannel(
		OutHit,
		RopeSocketStart,
		RopeSocketEnd,
		RopeQuat,
		ECollisionChannel::ECC_GameTraceChannel3,
		box,
		IngoreParams
		) };

	if (bDebugMode) {
		FVector CenterPoint{
				UKismetMathLibrary::VLerp(RopeSocketStart,RopeSocketEnd,0.5f)
		};

		UKismetSystemLibrary::DrawDebugBox(
			GetWorld(),
			CenterPoint,
			box.GetExtent(),
			isRopePoint ? FLinearColor::Green : FLinearColor::Red,
			RopeLookAtRot,
			1.0f,
			1.0f
		);
	}

	return isRopePoint;
}

void UTravelingComp::LandingHandle()
{
	ACharacter* CharRef = Cast<ACharacter>(GetOwner());
	float Z_Speed = CharRef->GetVelocity().Z;	

	if (FMath::Abs(Z_Speed) > 250.f)
	{
		CharRef->PlayAnimMontage(NormalLandingMontage);
		UAnimInstance* AnimInstance = CharRef->GetMesh()->GetAnimInstance();
		AnimInstance->Montage_SetEndDelegate(MontageEndDelegate);
		if (AnimInstance->Montage_IsPlaying(NormalLandingMontage))
			LockActionComp->LockInputMovement("Landing");

	}

	
}

void UTravelingComp::MontageEndHandler()
{
	LockActionComp->UnlockInputMovement("Landing");
}

