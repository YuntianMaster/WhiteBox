// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerActionsComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interact.h"
#include "Structure/FTakeDownAnimStruct.h"
#include "Enum/InputDirection.h"
#include "MotionWarpingComponent.h"
#include "AIController.h"
#include "Player/ShadowActor.h"
#include "Enemy/TakeDownComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Combat/WeaponSystemComp.h"
#include "MotionWarping.h"
#include "Player/PlayerCharacter.h"
#include "Enemy/TakeDownComponent.h"
#include "Interface/MainPlayerInterface.h"

// Sets default values for this component's properties
UPlayerActionsComponent::UPlayerActionsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
	// ...
}


// Called when the game starts
void UPlayerActionsComponent::BeginPlay()
{
	Super::BeginPlay();
	CharacterRef = GetOwner<ACharacter>();
	MovementComp = CharacterRef->GetCharacterMovement();
	if (!CharacterRef->Implements<UMainPlayerInterface>()) return;
	IplayerRef = Cast<IMainPlayerInterface>(CharacterRef);
	MovementComp->MaxWalkSpeed = WalkSpeed;
	WeaponSystemComp = CharacterRef->GetComponentByClass<UWeaponSystemComp>();
	MotionWarpingComp = CharacterRef->GetComponentByClass<UMotionWarpingComponent>();
	// ...
	
}


// Called every frame
void UPlayerActionsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	APlayerCameraManager* CamMgr = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	TakeDownTargetCheck();
		

	if (CamMgr && CharacterRef)
	{
		FRotator CameraRot = CamMgr->GetCameraRotation();
		FRotator CharacterRot = CharacterRef->GetActorRotation();

		// �ؼ�����
		PlayerRotator = (CameraRot - CharacterRot).GetNormalized();


		FVector MovementVectorRef = CharacterRef->GetLastMovementInputVector();
		FRotator NewRot1{ UKismetMathLibrary::MakeRotFromX(MovementVectorRef) };
		PlayerRotator_Input = NewRot1 - CharacterRot ;
		
	}

	
	// ...

	if(MovementComp->IsCrouching())
	{
		bIsCrouchTopBlock = CheckingCrouch();
		if(!bIsCrouchTopBlock&& !bIsCrouching)
			CrouchingEndHandle();
	}
}

void UPlayerActionsComponent::Sprint()
{
	if (!IplayerRef->HasEnoughStamina(SprintStamniaCost))
	{
		GoWalk();
		return;
	}
	if(Cast<APlayerCharacter>(CharacterRef)->PlayerStats == EPlayerStates::SwordDFence)
		return;
	if (MovementComp->Velocity.Equals(FVector::ZeroVector, 1)) return;
		MovementComp->MaxWalkSpeed = SprintSpeed;
		OnSprintDelegate.Broadcast(SprintStamniaCost);
}

void UPlayerActionsComponent::GoWalk()
{
	MovementComp->MaxWalkSpeed = WalkSpeed;
}

void UPlayerActionsComponent::Roll()
{

	if (!CharacterRef->Implements<UMainPlayerInterface>()) return;
	IMainPlayerInterface* PlayerRef{ Cast<IMainPlayerInterface>(CharacterRef) };
	if (!PlayerRef->HasEnoughStamina(RollStamniaCost)) return;
	OnSprintDelegate.Broadcast(RollStamniaCost);
	if (bIsRollActive) return;
	bIsRollActive = true;
	Cast<APlayerCharacter>(CharacterRef)->PlayerStats = EPlayerStates::Rolling;
	FVector RollDirection{
		CharacterRef->GetCharacterMovement()->Velocity.Length() < 1.f ?
			CharacterRef->GetActorForwardVector() :
			CharacterRef->GetLastMovementInputVector().GetSafeNormal()
	};
	const float ActorYaw = CharacterRef->GetActorForwardVector().Rotation().Yaw;
	const float InputYaw = RollDirection.GetSafeNormal2D().Rotation().Yaw;

	const float DeltaSigned = FMath::FindDeltaAngleDegrees(ActorYaw, InputYaw); // -180~180
	const float Angle360 = FRotator::ClampAxis(DeltaSigned);                     // 0~360
	UE_LOG(LogTemp, Warning, TEXT("last input yaw,%f"), Angle360);

	if (bIsComboRoll)
		RollIndex++;
	bIsComboRoll = false;
	//UE_LOG(LogTemp, Warning, TEXT("yaw::%f"), RotatYaw);
	int CounterMax = WeaponSystemComp->CurrentWeapon->Roll_F.Num();
	RollIndex = UKismetMathLibrary::Wrap(RollIndex, -1, CounterMax - 1);
	UE_LOG(LogTemp, Warning, TEXT("last input,%s"), *RollDirection.ToString());
	UE_LOG(LogTemp, Warning, TEXT("last input length,%f"), RollDirection.Length());
	float Duration = 0;
	//  不同角度播放不同翻滚动画
	if (Angle360 < 60)
	{
		Duration = CharacterRef->PlayAnimMontage(WeaponSystemComp->CurrentWeapon->Roll_F[RollIndex]);
		MotionWarpingComp->AddOrUpdateWarpTargetFromLocation(
			"Roll",
			CharacterRef->GetActorLocation() +
			RollDirection * 350
		);
	}
		
	else if (Angle360 < 120)
		Duration = CharacterRef->PlayAnimMontage(WeaponSystemComp->CurrentWeapon->Roll_R[RollIndex]);
	else if (Angle360 < 240)
	{

		
		Duration = CharacterRef->PlayAnimMontage(WeaponSystemComp->CurrentWeapon->Roll_B[RollIndex]);
		MotionWarpingComp->AddOrUpdateWarpTargetFromLocation(
			"Roll",
			CharacterRef->GetActorLocation() + RollDirection * 350
		);

	}
	else if (Angle360 < 300)
	{
		Duration = CharacterRef->PlayAnimMontage(WeaponSystemComp->CurrentWeapon->Roll_L[RollIndex]);
	}
	else {

		Duration = CharacterRef->PlayAnimMontage(WeaponSystemComp->CurrentWeapon->Roll_F[RollIndex]);
		MotionWarpingComp->AddOrUpdateWarpTargetFromLocation(
			"Roll",
			CharacterRef->GetActorLocation() + 
			RollDirection * 350
		);
	}
	//0.6动画后，可以继续输入
	CharacterRef->GetWorldTimerManager().SetTimer(
		RollComboTimeHandler,
		this,
		&UPlayerActionsComponent::ComboRollAnim,
		Duration*0.6,
		false
	);
	FLatentActionInfo RollingInfo(
		0,
		501,
		TEXT("EndRollingHandle"),
		this
		);
	UKismetSystemLibrary::RetriggerableDelay(GetWorld(), Duration * 0.8, RollingInfo);
	
	//1动画后，结束翻滚
	CharacterRef->GetWorldTimerManager().ClearTimer(RollFinishTimeHandler);
	CharacterRef->GetWorldTimerManager().SetTimer(
		RollFinishTimeHandler,
		this,
		&UPlayerActionsComponent::FinishRollAnim,
		Duration*1.2,
		false
	);
	

}

void UPlayerActionsComponent::FinishRollAnim()
{
	bIsComboRoll = false;
	RollIndex = 0;
	bIsRollActive = false;
}

void UPlayerActionsComponent::UpdatePostRollTurning()
{
		
		CharacterRef->bUseControllerRotationYaw = true;
}

void UPlayerActionsComponent::ComboRollAnim()
{
	UE_LOG(LogTemp, Warning, TEXT("cobom"));
	bIsComboRoll = true;
	bIsRollActive = false;
	
}

void UPlayerActionsComponent::EndRollingHandle()
{
	Cast<APlayerCharacter>(CharacterRef)->PlayerStats = EPlayerStates::CharacterNoneStats;

	UE_LOG(LogTemp, Warning, TEXT("Rolling over"));
}



void UPlayerActionsComponent::PlayerInteraceHandle()
{
	TArray<AActor*> actors;
	GetOwner()->GetOverlappingActors(actors);

	//UE_LOG(LogTemp, Warning, TEXT("START , %s"), *actors[0]->GetName());
	if (actors.Num() == 0) return;
	for (AActor* actor : actors) {

		if (actor->Implements<UInteract>())
		{
			IInteract* interactRef = Cast<IInteract>(actor);

			interactRef->InteractHandle();

		}
	}
}
void UPlayerActionsComponent::CrouchingStartHandle()
{
	CharacterRef->Crouch();
	//MovementComp->bOrientRotationToMovement = false;
	FVector MovementVectorRef = CharacterRef->GetLastMovementInputVector();
	FRotator CharacterRot = CharacterRef->GetActorRotation();
	FRotator NewRot{ UKismetMathLibrary::MakeRotFromX(MovementVectorRef) };
	float Delta = GetWorld() ? GetWorld()->DeltaTimeSeconds : 0.01f;
	GetOwner()->GetWorldTimerManager().SetTimer(
		ZoomOutHandle,
		this,
		&UPlayerActionsComponent::CamZoonOut,
		Delta,
		true
	);
	
}

void UPlayerActionsComponent::CrouchingEndHandle()
{
	
	if (bIsCrouchTopBlock) return;
	CharacterRef->UnCrouch();
	float Delta = GetWorld() ? GetWorld()->DeltaTimeSeconds : 0.01f;
	GetOwner()->GetWorldTimerManager().SetTimer(
		ZoomInHandle,
		this,
		&UPlayerActionsComponent::CamZoonIn,
		Delta,
		true
	);
	//MovementComp->bOrientRotationToMovement = true;
}

void UPlayerActionsComponent::CamZoonIn()
{

	ACharacter* CharRef{ GetOwner<ACharacter>() };
	USpringArmComponent* SpringArmRef{ CharRef->GetComponentByClass<USpringArmComponent>() };

	if (!SpringArmRef) return;
	GetOwner()->GetWorldTimerManager().ClearTimer(ZoomOutHandle);
	float Delta = GetWorld() ? GetWorld()->DeltaTimeSeconds : 0.01f;
	float currentCameraLength = SpringArmRef->TargetArmLength;
	SpringArmRef->TargetArmLength = FMath::FInterpTo(currentCameraLength, 400.0f, Delta, ZoomSpeed);
	if (FMath::IsNearlyEqual(SpringArmRef->TargetArmLength, 400.0f, 0.5f))
		GetOwner()->GetWorldTimerManager().ClearTimer(ZoomInHandle);
}

void UPlayerActionsComponent::CamZoonOut()
{

	ACharacter* CharRef{ GetOwner<ACharacter>() };
	USpringArmComponent* SpringArmRef{ CharRef->GetComponentByClass<USpringArmComponent>() };

	if (!SpringArmRef) return;
	GetOwner()->GetWorldTimerManager().ClearTimer(ZoomInHandle);
	float Delta = GetWorld() ? GetWorld()->DeltaTimeSeconds : 0.01f;
	float currentCameraLength = SpringArmRef->TargetArmLength;
	SpringArmRef->TargetArmLength = FMath::FInterpTo(currentCameraLength, ZoomCameraLength, Delta, ZoomSpeed);
	if (FMath::IsNearlyEqual(SpringArmRef->TargetArmLength, ZoomCameraLength, 0.5f))
		GetOwner()->GetWorldTimerManager().ClearTimer(ZoomOutHandle);
	

}

bool UPlayerActionsComponent::CheckingCrouch()
{
	FHitResult OutHit;
	USkeletalMeshComponent* SkeletonRef{ GetOwner()->FindComponentByClass<USkeletalMeshComponent>()};
	FVector Start = SkeletonRef->GetSocketLocation(FName("head"));
	FQuat Quat = SkeletonRef->GetSocketQuaternion(FName("head"));
	
	FVector BoxHalfExtent{ 50.f,50.f,100.f };
	BoxHalfExtent /= 2;
	FCollisionShape box{ FCollisionShape::MakeBox(BoxHalfExtent) };
	FCollisionQueryParams IngoreParams{ FName{"Igorna"}, false, GetOwner() };
	bool checkResult = GetWorld()->SweepSingleByChannel(
		OutHit,
		Start,
		Start,
		FQuat::Identity,
		ECC_Pawn,
		box,
		IngoreParams
	);

	if(DebugMode)
	{
		UKismetSystemLibrary::DrawDebugBox(
			GetWorld(),
			Start,
			box.GetExtent(),
			checkResult ? FLinearColor::Green : FLinearColor::Red,
			FRotator::ZeroRotator,
			1.0f,
			1.0f
		);
	}

	return checkResult;
}

void UPlayerActionsComponent::PlayStartMontage()
{
	FVector PlayerForward = CharacterRef->GetActorForwardVector();
	FVector LastInput = CharacterRef->GetLastMovementInputVector();
	FVector Velocity = CharacterRef->GetCharacterMovement()->Velocity.GetSafeNormal();
	
	FRotator rot = UKismetMathLibrary::MakeRotFromX(LastInput);
	FRotator rot1 = UKismetMathLibrary::MakeRotFromX(PlayerForward);
	FRotator rot2 = rot - rot1;
	float Yaw = FRotator::NormalizeAxis(rot2.Yaw);
	PlayerDirectionInput currentDirection = PlayerDirectionInput::F_0;
	float Tolerance = 10.f;
}

void UPlayerActionsComponent::TakeDownTargetCheck()
{
	FHitResult OutHit;
	FCollisionShape Sphere{ FCollisionShape::MakeSphere(TakeDownCheckRadiu) };
	FVector Playerloc = GetOwner()->GetActorLocation();
	FCollisionQueryParams Ingore{ FName{"IgoreParater"},false,GetOwner() };
	bIsTakeDownchecked = GetWorld()->SweepSingleByChannel(
		OutHit,
		Playerloc,
		Playerloc,
		FQuat::Identity,
		ECC_GameTraceChannel4,
		Sphere,
		Ingore
	);
	TakeDownTarget = OutHit.GetActor();
	if (DebugMode)
	{
		UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Playerloc, TakeDownCheckRadiu);
	}
	if (TakeDownTarget) {
		TakeDownTarget->FindComponentByClass<UTakeDownComponent>()->SetUIVisable();
	}	

}

AActor* UPlayerActionsComponent::TakeDownAction()
{
	if (!bIsTakeDownchecked) return nullptr;
	AShadowActor* ShadowActor = Cast<AShadowActor>(GetWorld()->SpawnActor(ShadowRef, &GetOwner()->GetActorTransform()));
	if (ShadowActor)
	{
		ShadowActor->SpawnDefaultController();
		ShadowActor->RunBT(TakeDownMontage[0].Attack, TakeDownTarget->GetActorTransform());
		UE_LOG(LogTemp, Warning, TEXT("START111s"));
	}	

	if (TakeDownTarget)
	{
		UTakeDownComponent* TakeDownRef = TakeDownTarget->FindComponentByClass<UTakeDownComponent>();
		if (TakeDownRef)
		{
			TakeDownRef->UnderTakeDown(TakeDownMontage[0].UnderAttack);
		}
	}
	return TakeDownTarget;
}










