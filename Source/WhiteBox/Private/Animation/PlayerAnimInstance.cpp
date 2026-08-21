// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PlayerAnimInstance.h"
#include "Combat/WeaponSystemComp.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/PlayerCharacter.h"

void UPlayerAnimInstance::UpdateTheVelocity()
{
	
	APawn* Apawnref{ TryGetPawnOwner() };
	if (!IsValid(Apawnref)) return;
	FVector velocity{ Apawnref->GetVelocity() };
	CurrentVelocity = static_cast<float>(velocity.Length());
	//CurrentVelocity = velocity.Length();
	FRotator AimDirect = Apawnref->GetBaseAimRotation();
	FRotator ActorDirect = Apawnref->GetActorRotation();
	FRotator AimingRo = (AimDirect - ActorDirect).GetNormalized();
	AimingPitch = AimingRo.Pitch;
}

void UPlayerAnimInstance::UpdateOrientationData()
{
	VelocityLocomotionAngle = CalculateDirection(CachedOwnerVelocity, CachedOwnerRotation);
	LastFramePlayerYaw = PlayerYaw;
	PlayerYaw = CachedOwnerRotation.Yaw;
	DeltaPlayerYaw = PlayerYaw - LastFramePlayerYaw;
	APawn* Apawnref{ TryGetPawnOwner() };
	if (!IsValid(Apawnref))
	{
		return;
	}

	UCharacterMovementComponent* MovementComp = Apawnref->GetComponentByClass<UCharacterMovementComponent>();
	if (MovementComp)
	{
		CharacterAcceleration  = MovementComp->GetCurrentAcceleration();
		AcelectrationAngle = CalculateDirection(CharacterAcceleration, CachedOwnerRotation);

	}

}

void UPlayerAnimInstance::UpdateRootYawOffset(float DeltaTime)
{
	RootYawOffset = UKismetMathLibrary::FloatSpringInterp(
		RootYawOffset, 
		0.f, 
		YawSpringState,
		80.0f, 
		1.0f, 
		DeltaTime);
}


void UPlayerAnimInstance::AccumulateRootYawOffset()
{
	RootYawOffset -= DeltaPlayerYaw;
	RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset);

}

void UPlayerAnimInstance::ProcessTurnYawCurve()
{
	float bIsTurning = GetCurveValue(TEXT("IsTurning"));
	FrameTurnYawCurveValue = UKismetMathLibrary::SafeDivide(GetCurveValue(TEXT("root_rotation_Z")), bIsTurning);
	//FrameTurnYawCurveValue = UKismetMathLibrary::FClamp(FrameTurnYawCurveValue, -90.f, 90.f);
	float DeltaTurnYaw = FrameTurnYawCurveValue - LastFrameTurnYawCurveValue;

	

	if(bIsTurning>0.f)
	{
		if (LastFrameTurnYawCurveValue != 0.f)
			SetRootYawAngle(RootYawOffset - DeltaTurnYaw);
		UE_LOG(LogTemp, Warning, TEXT("RootYawOffset: %f"), RootYawOffset);
		UE_LOG(LogTemp, Warning, TEXT("FrameTurnYawCurveValue: %f"), FrameTurnYawCurveValue);
		UE_LOG(LogTemp, Warning, TEXT("LastFrameTurnYawCurveValue: %f"), LastFrameTurnYawCurveValue);
		UE_LOG(LogTemp, Warning, TEXT("bIsTurning: %f"), bIsTurning);

		
	}
	else
	{
		FrameTurnYawCurveValue = 0.f;
		LastFrameTurnYawCurveValue = 0.f;
	
	}
	LastFrameTurnYawCurveValue = FrameTurnYawCurveValue;
}



void UPlayerAnimInstance::UpdateEPlayerVelocityDirection()
{
	/*if (UKismetMathLibrary::Abs(VelocityLocomotionAngle) < FrontAngle + DeadZoneAngle) {

		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityForward;
	}
	else if (UKismetMathLibrary::Abs(VelocityLocomotionAngle) > BackAngle - DeadZoneAngle) {
		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityBackward;
	}
	else if (VelocityLocomotionAngle < 0.f) {
		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityLeft;
	}
	else if (VelocityLocomotionAngle > 0.f) {
		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityRight;
	}
	else {
		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityNone;
	}*/




	if (VelocityLocomotionAngle > -22.5f && VelocityLocomotionAngle <= 22.5f)
	{
		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityForward;
	}
	else if (VelocityLocomotionAngle > 22.5f && VelocityLocomotionAngle <= 67.5f)
	{
		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityFR;
	}
	else if (VelocityLocomotionAngle > 67.5f && VelocityLocomotionAngle <= 112.5f)
	{
		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityRight;
	}
	else if (VelocityLocomotionAngle > 112.5f && VelocityLocomotionAngle <= 157.5f)
	{
		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityBR;
	}
	else if (VelocityLocomotionAngle > 157.5f || VelocityLocomotionAngle <= -157.5f)
	{
		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityBackward;
	}
	else if (VelocityLocomotionAngle > -157.5f && VelocityLocomotionAngle <= -112.5f)
	{
		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityBL;
	}
	else if (VelocityLocomotionAngle > -112.5f && VelocityLocomotionAngle <= -67.5f)
	{
		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityLeft;
	}
	else if (VelocityLocomotionAngle > -67.5f && VelocityLocomotionAngle <= -22.5f)
	{
		CurrentVelocityDirection = EPlayerVelocityDirection::VelocityFL;
	}


}

void UPlayerAnimInstance::UpdateCharacterGateState()
{
	LastFrameGate = CurrentGate;
	CurrentGate = IncomingGate;
	if (CurrentGate == LastFrameGate)
		bIsGateChange = false;
	else
		bIsGateChange = true;
}

void UPlayerAnimInstance::UpdateDeltaLocation()
{
	APawn* Apawnref{ TryGetPawnOwner() };
	if (!Apawnref) return;
	FVector ActorLoc = Apawnref->GetActorLocation();
	

	PlayerDeltaLoc = FVector::Distance(ActorLoc, LastFrameActorLocation);
	LastFrameActorLocation = ActorLoc;
}

void UPlayerAnimInstance::UpdateAccelerationDirection()
{
	if (UKismetMathLibrary::Abs(AcelectrationAngle) < FrontAngle + DeadZoneAngle) {

		CurrentAcelectrationDirection = EPlayerVelocityDirection::VelocityForward;
	}
	else if (UKismetMathLibrary::Abs(AcelectrationAngle) > BackAngle - DeadZoneAngle) {
		CurrentAcelectrationDirection = EPlayerVelocityDirection::VelocityBackward;
	}
	else if (AcelectrationAngle < 0.f) {
		CurrentAcelectrationDirection = EPlayerVelocityDirection::VelocityLeft;
	}
	else if (AcelectrationAngle > 0.f) {
		CurrentAcelectrationDirection = EPlayerVelocityDirection::VelocityRight;
	}
	else {
		CurrentAcelectrationDirection = EPlayerVelocityDirection::VelocityNone;
	}

}

void UPlayerAnimInstance::SetRootYawAngle(float NewRootYawAngle)
{
	RootYawOffset = UKismetMathLibrary::NormalizeAxis(NewRootYawAngle);
	
}



void UPlayerAnimInstance::HandleUpdateTarget(AActor* NewTargetActorRef)
{
	bIsIncombat = IsValid(NewTargetActorRef);

}

void UPlayerAnimInstance::UpdateDirection()
{
	APawn* Apawnref{ TryGetPawnOwner() };
	if (!Apawnref) return;
	//if (!bIsIncombat) return;

	CurrentDirect = CalculateDirection(Apawnref->GetVelocity(), Apawnref->GetActorRotation());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);

	if (APawn* PawnRef = TryGetPawnOwner())
	{
		CachedOwnerVelocity = PawnRef->GetVelocity();
		CachedOwnerRotation = PawnRef->GetActorRotation();
		
	}




}

void UPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	UpdateOrientationData();
	UpdateEPlayerVelocityDirection();
	UpdateAccelerationDirection();
	UpdateCharacterGateState();
	UpdateDeltaLocation();
	//UpdateRootYawOffset(DeltaSeconds);
}
