#include "Camera/Node/SideFocusCameraNode.h"

#include "Components/ActorComponent.h"
#include "Engine/LocalPlayer.h"
#include "Core/CameraEvaluationContext.h"
#include "Core/CameraNodeEvaluator.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SideFocusCameraNode)

namespace UE::Cameras
{

class FSideFocusCameraNodeEvaluator : public FCameraNodeEvaluator
{
	UE_DECLARE_CAMERA_NODE_EVALUATOR(WHITEBOX_API, FSideFocusCameraNodeEvaluator)

protected:
	virtual void OnInitialize(const FCameraNodeEvaluatorInitializeParams& Params, FCameraNodeEvaluationResult& OutResult) override;
	virtual void OnRun(const FCameraNodeEvaluationParams& Params, FCameraNodeEvaluationResult& OutResult) override;
};

UE_DEFINE_CAMERA_NODE_EVALUATOR(FSideFocusCameraNodeEvaluator)

void FSideFocusCameraNodeEvaluator::OnInitialize(const FCameraNodeEvaluatorInitializeParams& Params, FCameraNodeEvaluationResult& OutResult)
{
}

void FSideFocusCameraNodeEvaluator::OnRun(const FCameraNodeEvaluationParams& Params, FCameraNodeEvaluationResult& OutResult)
{
	const USideFocusCameraNode* SideFocusNode = GetCameraNodeAs<USideFocusCameraNode>();
	if (!SideFocusNode)
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

	APlayerCharacter* CharRef = Cast<APlayerCharacter>(OwnerActor);
	AActor* TargetActor = CharRef ? CharRef->TargetActor : nullptr;
	if (!CharRef || !IsValid(TargetActor))
	{
		return;
	}

	const FVector PlayerLocation = CharRef->GetActorLocation();
	const FVector TargetLocation = TargetActor->GetActorLocation();

	const FVector Mid = (PlayerLocation + TargetLocation) * 0.5f;
	FVector Dir = TargetLocation - PlayerLocation;
	Dir.Z = 0.f;
	if (Dir.IsNearlyZero())
	{
		return;
	}
	Dir.Normalize();

	const FVector Right = FVector::CrossProduct(Dir, FVector::UpVector).GetSafeNormal();

	float AspectRatio = 16.f / 9.f;
	if (APlayerController* PlayerController = Params.EvaluationContext->GetPlayerController())
	{
		if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (LocalPlayer->ViewportClient)
			{
				FVector2D ViewportSize;
				LocalPlayer->ViewportClient->GetViewportSize(ViewportSize);
				if (ViewportSize.Y > 0.f)
				{
					AspectRatio = ViewportSize.X / ViewportSize.Y;
				}
			}
		}
	}

	const float VerticalFOV = OutResult.CameraPose.GetFieldOfView();
	const float HalfVFovRad = FMath::DegreesToRadians(VerticalFOV * 0.5f);
	const float HalfHFovRad = FMath::Atan(FMath::Tan(HalfVFovRad) * AspectRatio);
	const float SpanH = FVector::Dist2D(PlayerLocation, TargetLocation);
	float DistH = (SpanH * 0.5f) / FMath::Tan(HalfHFovRad);

	UE_LOG(LogTemp, Log, TEXT("AspectRatio: %f, VerticalFOV: %f, HalfHFovRad: %f, SpanH: %f, DistH: %f"), AspectRatio, VerticalFOV, FMath::RadiansToDegrees(HalfHFovRad), SpanH, DistH);


	DistH = FMath::Max(DistH, SideFocusNode->Distance);
	FVector CameraLocation = Mid + Right * DistH;
	CameraLocation += SideFocusNode->OffsetLocation;

	const FRotator CamRot = UKismetMathLibrary::FindLookAtRotation(CameraLocation, Mid) + SideFocusNode->OffsetRotation;
	OutResult.CameraPose.SetTransform(FTransform3d(CamRot, CameraLocation));
}

} // namespace UE::Cameras

FCameraNodeEvaluatorPtr USideFocusCameraNode::OnBuildEvaluator(FCameraNodeEvaluatorBuilder& Builder) const
{
	using namespace UE::Cameras;
	return Builder.BuildEvaluator<FSideFocusCameraNodeEvaluator>();
}
