#include "Camera/Evalutor/CameraLockYawPitch.h"

#include "Components/ActorComponent.h"
#include "Core/CameraEvaluationContext.h"
#include "Core/CameraNodeEvaluator.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Nodes/Input/Input2DCameraNode.h"
#include "Player/PlayerCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CameraLockYawPitch)

// GameplayCameras 未导出该符号，游戏模块子类链接时需要提供实现。
void UE::Cameras::FInput2DCameraNodeEvaluator::OnSerialize(const FCameraNodeEvaluatorSerializeParams& Params, FArchive& Ar)
{
	Ar << InputValue;
}

namespace UE::Cameras
{

class FCameraLockYawPitchInputEvaluator : public FInput2DCameraNodeEvaluator
{
	UE_DECLARE_CAMERA_NODE_EVALUATOR_EX(WHITEBOX_API, FCameraLockYawPitchInputEvaluator, FInput2DCameraNodeEvaluator)

protected:
	virtual void OnRun(const FCameraNodeEvaluationParams& Params, FCameraNodeEvaluationResult& OutResult) override;
	virtual void OnInitialize(
		const FCameraNodeEvaluatorInitializeParams& Params,
		FCameraNodeEvaluationResult& OutResult) override;
};

UE_DEFINE_CAMERA_NODE_EVALUATOR(FCameraLockYawPitchInputEvaluator)

void FCameraLockYawPitchInputEvaluator::OnInitialize(const FCameraNodeEvaluatorInitializeParams& Params, FCameraNodeEvaluationResult& OutResult)
{
	InputValue = FVector2d::ZeroVector;
	if (const FCameraNodeEvaluationResult* LastResult = Params.LastActiveCameraRigInfo.LastResult)
	{
		const FRotator3d& LastRotation = LastResult->CameraPose.GetRotation();
		InputValue = FVector2d(LastRotation.Yaw, LastRotation.Pitch);
	}
}

void FCameraLockYawPitchInputEvaluator::OnRun(const FCameraNodeEvaluationParams& Params, FCameraNodeEvaluationResult& OutResult)
{
	const UCameraLockYawPitch* LockNode = GetCameraNodeAs<UCameraLockYawPitch>();
	if (!LockNode)
	{
		return;
	}

	UObject* ContextOwner = Params.EvaluationContext->GetOwner();
	AActor* OwnerActor = nullptr;
	if (UActorComponent* ContextComponent = Cast<UActorComponent>(ContextOwner))
	{
		OwnerActor = ContextComponent->GetOwner();
	}
	else
	{
		OwnerActor = Cast<AActor>(ContextOwner);
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(OwnerActor);
	if (!Player || !IsValid(Player->TargetActor))
	{
		return;
	}

	const FVector PlayerLocation = Player->GetActorLocation();
	FVector TargetLocation = Player->TargetActor->GetActorLocation();
	TargetLocation.Z -= LockNode->TargetHeightOffset;

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetLocation);
	TargetRotation.Pitch = FMath::Clamp(TargetRotation.Pitch, LockNode->MinPitch.Value, LockNode->MaxPitch.Value);
	//UE_LOG(LogTemp, Warning, TEXT("TargetRotation: MinPitch.Value=%f MaxPitch.Value=%f"), LockNode->MinPitch.Value, LockNode->MaxPitch.Value);
	// Boom Arm: FRotator(YawPitch.Y, YawPitch.X, 0) => X=Yaw, Y=Pitch
	FVector2d TargetYawPitch(TargetRotation.Yaw, TargetRotation.Pitch);

	if (LockNode->bInterpolate && Params.DeltaTime > 0.f)
	{
		// Yaw 在 ±180° 处会回绕，FInterpTo 会走长路径导致突然转一圈
		const float DeltaYaw = FMath::FindDeltaAngleDegrees(InputValue.X, TargetYawPitch.X);
		TargetYawPitch.X = FRotator::NormalizeAxis(
			InputValue.X + FMath::FInterpTo(0.f, DeltaYaw, Params.DeltaTime, LockNode->InterpSpeed));
		TargetYawPitch.Y = FMath::FInterpTo(InputValue.Y, TargetYawPitch.Y, Params.DeltaTime, LockNode->InterpSpeed);
	}

	InputValue = TargetYawPitch;
}

} // namespace UE::Cameras

FCameraNodeEvaluatorPtr UCameraLockYawPitch::OnBuildEvaluator(FCameraNodeEvaluatorBuilder& Builder) const
{
	using namespace UE::Cameras;
	return Builder.BuildEvaluator<FCameraLockYawPitchInputEvaluator>();
}
