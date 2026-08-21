#pragma once

#include "Core/CameraNodeEvaluator.h"
#include "Nodes/Input/Input2DCameraNode.h"

namespace UE::Cameras
{

/**
 * 可复用的 Input2D Evaluator 基类：OnInitialize 从上一帧 Camera Pose 恢复 Yaw/Pitch 到 InputValue。
 * 具体节点 Evaluator 继承本类，只需 override OnRun。
 *
 * 用法示例：
 *   class FMyInputEvaluator : public FWhiteBoxInput2DCameraNodeEvaluator
 *   {
 *       UE_DECLARE_CAMERA_NODE_EVALUATOR_EX(WHITEBOX_API, FMyInputEvaluator, FWhiteBoxInput2DCameraNodeEvaluator)
 *   protected:
 *       virtual void OnRun(...) override;
 *   };
 */
class WHITEBOX_API FWhiteBoxInput2DCameraNodeEvaluator : public FInput2DCameraNodeEvaluator
{
public:
	using FInput2DCameraNodeEvaluator::FInput2DCameraNodeEvaluator;

protected:
	virtual void OnInitialize(
		const FCameraNodeEvaluatorInitializeParams& Params,
		FCameraNodeEvaluationResult& OutResult) override;
};

} // namespace UE::Cameras
