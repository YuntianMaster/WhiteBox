#pragma once

#include "Core/CameraParameters.h"
#include "Math/MathFwd.h"
#include "Nodes/Input/CameraRigInputSlotTypes.h"
#include "Nodes/Input/Input2DCameraNode.h"

#include "WhiteBoxInputAxisBinding2DCameraNode.generated.h"

class UInputAction;

/**
 * Input Axis Binding 2D 的游戏模块替代节点：行为与引擎内置节点一致，
 * 并在进入 Rig 时从上一帧 Camera Pose 初始化 InputValue（Yaw/Pitch）。
 * 在 Rig 中替换 Built-in Input Axis Binding 2D 使用。
 */
UCLASS(meta = (CameraNodeCategories = "Input"))
class WHITEBOX_API UWhiteBoxInputAxisBinding2DCameraNode : public UInput2DCameraNode
{
	GENERATED_BODY()

public:

	UWhiteBoxInputAxisBinding2DCameraNode(const FObjectInitializer& ObjectInitializer);

	/** The axis input action(s) to read from. */
	UPROPERTY(EditAnywhere, Category = "Input")
	TArray<TObjectPtr<UInputAction>> AxisActions;

	/** Whether to revert the X axis. */
	UPROPERTY(EditAnywhere, Category = "Input Processing")
	FBooleanCameraParameter RevertAxisX = false;

	/** Whether to revert the Y axis. */
	UPROPERTY(EditAnywhere, Category = "Input Processing")
	FBooleanCameraParameter RevertAxisY = false;

	/** A multiplier to use on the input values. */
	UPROPERTY(EditAnywhere, Category = "Input Processing")
	FVector2dCameraParameter Multiplier;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (ShowOnlyInnerProperties))
	FCameraRigInputSlotParameters InputSlotParameters;

	UPROPERTY(EditAnywhere, Category = "Input")
	FCameraParameterClamping ClampX;

	UPROPERTY(EditAnywhere, Category = "Input")
	FCameraParameterClamping ClampY;

	UPROPERTY(EditAnywhere, Category = "Input")
	FCameraParameterNormalization NormalizeX;

	UPROPERTY(EditAnywhere, Category = "Input")
	FCameraParameterNormalization NormalizeY;

protected:
	virtual FCameraNodeEvaluatorPtr OnBuildEvaluator(FCameraNodeEvaluatorBuilder& Builder) const override;
};
