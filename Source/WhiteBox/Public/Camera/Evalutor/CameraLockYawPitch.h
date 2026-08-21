#pragma once

#include "Nodes/Input/Input2DCameraNode.h"
#include "Core/CameraParameters.h"
#include "CameraLockYawPitch.generated.h"

/**
 * 锁定目标时输出 Yaw/Pitch，供 Boom Arm 的 Input 槽使用。
 * 在 LockTarget Rig 里：把本节点接到 Boom Arm 的 Input Slot（替换 Input Axis Binding）。
 */
UCLASS(meta = (CameraNodeCategories = "Input"))
class WHITEBOX_API UCameraLockYawPitch : public UInput2DCameraNode
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Camera|Lock")
	float TargetHeightOffset = 100.f;

	UPROPERTY(EditAnywhere, Category = "Camera|Lock")
	FFloatCameraParameter  MinPitch;

	UPROPERTY(EditAnywhere, Category = "Camera|Lock")
	FFloatCameraParameter  MaxPitch ;

	UPROPERTY(EditAnywhere, Category = "Camera|Lock")
	bool bInterpolate = true;

	UPROPERTY(EditAnywhere, Category = "Camera|Lock", meta = (EditCondition = "bInterpolate"))
	float InterpSpeed = 6.f;



protected:
	virtual FCameraNodeEvaluatorPtr OnBuildEvaluator(FCameraNodeEvaluatorBuilder& Builder) const override;
};
