#include "Camera/Evalutor/WhiteBoxInput2DCameraNodeEvaluator.h"

namespace UE::Cameras
{

void FWhiteBoxInput2DCameraNodeEvaluator::OnInitialize(
	const FCameraNodeEvaluatorInitializeParams& Params,
	FCameraNodeEvaluationResult& OutResult)
{
	InputValue = FVector2d::ZeroVector;
	if (const FCameraNodeEvaluationResult* LastResult = Params.LastActiveCameraRigInfo.LastResult)
	{
		const FRotator3d& LastRotation = LastResult->CameraPose.GetRotation();
		InputValue = FVector2d(LastRotation.Yaw, LastRotation.Pitch);
		
	}
}

} // namespace UE::Cameras
