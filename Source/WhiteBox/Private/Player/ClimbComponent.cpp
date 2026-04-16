 // Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ClimbComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "Engine/CollisionProfile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/UnlockActionComponent.h"
#include "MotionWarpingComponent.h"

// Sets default values for this component's properties
UClimbComponent::UClimbComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UClimbComponent::BeginPlay()
{
	Super::BeginPlay();
	ClimbEnd.BindUFunction(this, "MontageEndHandle");
	LockComp = GetOwner()->FindComponentByClass<UUnlockActionComponent>();
	// ...
	
}


// Called every frame
void UClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UClimbComponent::checkblocker()
{
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Start + GetOwner()->GetActorForwardVector() * 70.f;

	EObjectTypeQuery checktype = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(checktype);
	TArray<AActor*> Ingores;
	FHitResult OutHits;

	FVector TargetUp = FVector::Zero();
	FVector TargetForward = FVector::Zero();

	bDistanceCheck = false;
	for (size_t i = 0; i < 9; i++)
	{
		Start += GetOwner()->GetActorUpVector() * 25.f;
		End += GetOwner()->GetActorUpVector() * 25.f;

		bool result = UKismetSystemLibrary::SphereTraceSingleForObjects(
			GetWorld(),
			Start,
			End,
			10.f,
			ObjectTypes,
			false,
			Ingores,
			bDebugMode ? EDrawDebugTrace::ForDuration: EDrawDebugTrace::None,
			OutHits,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			5.f
		);

		if (!result)
		{
			if (i!= 0)
			{
				bDistanceCheck = true;
				HighLevel = i;
			}
			break;
		}
	}

	if (!bDistanceCheck) return;

	FVector v_End = GetOwner()->GetActorLocation();
	FVector v_Start = v_End + GetOwner()->GetActorUpVector() * 300.f;

	for (size_t i = 0; i < 5; i++)
	{
		v_Start += GetOwner()->GetActorForwardVector() * 25.f;
		v_End += GetOwner()->GetActorForwardVector() * 25.f;

		bool result = UKismetSystemLibrary::SphereTraceSingleForObjects(
			GetWorld(),
			v_Start,
			v_End,
			10.f,
			ObjectTypes,
			false,
			Ingores,
			bDebugMode ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
			OutHits,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			5.f
		);

		if (result) {
			if(bDebugMode)
				UKismetSystemLibrary::DrawDebugCircle(GetWorld(), OutHits.ImpactPoint, 20.f, 12, FLinearColor::Blue, 5.0f);
			PlayClimbMontage(HighLevel, OutHits.ImpactPoint);
			break;
		}

		
	}
}

void UClimbComponent::PlayClimbMontage(int highIndex,FVector Loc)
{
	//UE_LOG(LogTemp, Warning, TEXT("123123"));
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	Loc = Loc + GetOwner()->GetActorForwardVector() * 25.f /*+ GetOwner()->GetActorUpVector() * 25.f*/;

	UAnimMontage* ClimbingAnim = LowClimb;
	switch (highIndex)
	{
	case 1:
		ClimbingAnim = LowClimb;
		break;
	case 2:
		ClimbingAnim = LowClimb;
		break;
	case 3:
		ClimbingAnim = HighClimb;
		break;
	case 4:
		ClimbingAnim = HighClimb;
		break;
	case 5:
		ClimbingAnim = JumpClimb;
		break;
	case 6:
		ClimbingAnim = JumpClimb;
		break;
	case 7:
		ClimbingAnim = JumpClimb;
		break;
	case 8:
		ClimbingAnim = JumpClimb;
		break;
	default:
		break;
	}

	Character->PlayAnimMontage(ClimbingAnim);
	LockComp->LockInputMovement("Climbing");
	GetOwner()->FindComponentByClass<UMotionWarpingComponent>()->AddOrUpdateWarpTargetFromLocationAndRotation("Climb", Loc, Character->GetActorRotation());
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	AnimInstance->Montage_SetEndDelegate(ClimbEnd, ClimbingAnim);
	//if (AnimInstance->Montage_IsPlaying(ClimbingAnim))
		

}

void UClimbComponent::MontageEndHandle()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	USkeletalMeshComponent* MeshComp =  Character->GetMesh();
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	LockComp->UnlockInputMovement("Climbing");

}

