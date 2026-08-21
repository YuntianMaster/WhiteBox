#include "Camera/Input/WhiteBoxInputAxisBinding2DCameraNode.h"

#include "Camera/Evalutor/WhiteBoxInput2DCameraNodeEvaluator.h"
#include "Components/InputComponent.h"
#include "Core/CameraEvaluationContext.h"
#include "Core/CameraParameterReader.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Actor.h"
#include "InputAction.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WhiteBoxInputAxisBinding2DCameraNode)

namespace WhiteBox::InputAxisBinding::Private
{

static double ClampAxisValue(const FCameraParameterClamping& Clamping, double Value)
{
	if (Clamping.bClampMin && Value < Clamping.MinValue)
	{
		Value = Clamping.MinValue;
	}
	if (Clamping.bClampMax && Value > Clamping.MaxValue)
	{
		Value = Clamping.MaxValue;
	}
	return Value;
}

static double NormalizeAxisValue(const FCameraParameterNormalization& Normalization, double Value)
{
	if (Normalization.bNormalize && Normalization.MaxValue > 0)
	{
		while (Value > Normalization.MaxValue)
		{
			Value -= Normalization.MaxValue;
		}
	}
	return Value;
}

} // namespace WhiteBox::InputAxisBinding::Private

namespace UE::Cameras
{

class FWhiteBoxInputAxisBinding2DCameraNodeEvaluator : public FWhiteBoxInput2DCameraNodeEvaluator
{
	UE_DECLARE_CAMERA_NODE_EVALUATOR_EX(
		WHITEBOX_API,
		FWhiteBoxInputAxisBinding2DCameraNodeEvaluator,
		FWhiteBoxInput2DCameraNodeEvaluator)

protected:

	virtual void OnInitialize(
		const FCameraNodeEvaluatorInitializeParams& Params,
		FCameraNodeEvaluationResult& OutResult) override;
	virtual void OnRun(
		const FCameraNodeEvaluationParams& Params,
		FCameraNodeEvaluationResult& OutResult) override;

private:

	TObjectPtr<UEnhancedInputComponent> InputComponent;
	TArray<FEnhancedInputActionValueBinding*> AxisValueBindings;

	TCameraParameterReader<bool> RevertAxisXReader;
	TCameraParameterReader<bool> RevertAxisYReader;
	TCameraParameterReader<FVector2d> MultiplierReader;
};

UE_DEFINE_CAMERA_NODE_EVALUATOR(FWhiteBoxInputAxisBinding2DCameraNodeEvaluator)

void FWhiteBoxInputAxisBinding2DCameraNodeEvaluator::OnInitialize(
	const FCameraNodeEvaluatorInitializeParams& Params,
	FCameraNodeEvaluationResult& OutResult)
{
	Super::OnInitialize(Params, OutResult);

	UObject* ContextOwner = Params.EvaluationContext->GetOwner();
	if (ContextOwner)
	{
		if (AActor* ContextOwnerActor = Cast<AActor>(ContextOwner))
		{
			InputComponent = Cast<UEnhancedInputComponent>(ContextOwnerActor->InputComponent);
		}
		else if (AActor* OuterActor = ContextOwner->GetTypedOuter<AActor>())
		{
			InputComponent = Cast<UEnhancedInputComponent>(OuterActor->InputComponent);
		}
	}

	const UWhiteBoxInputAxisBinding2DCameraNode* AxisBindingNode =
		GetCameraNodeAs<UWhiteBoxInputAxisBinding2DCameraNode>();

	RevertAxisXReader.Initialize(AxisBindingNode->RevertAxisX);
	RevertAxisYReader.Initialize(AxisBindingNode->RevertAxisY);
	MultiplierReader.Initialize(AxisBindingNode->Multiplier);

	if (InputComponent)
	{
		for (TObjectPtr<UInputAction> AxisAction : AxisBindingNode->AxisActions)
		{
			FEnhancedInputActionValueBinding* AxisValueBinding = &InputComponent->BindActionValue(AxisAction);
			AxisValueBindings.Add(AxisValueBinding);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("No input component found on context owner '%s' for node '%s' in '%s'."),
			*GetNameSafe(ContextOwner),
			*GetNameSafe(AxisBindingNode),
			*GetNameSafe(AxisBindingNode ? AxisBindingNode->GetOutermost() : nullptr));
	}
}

void FWhiteBoxInputAxisBinding2DCameraNodeEvaluator::OnRun(
	const FCameraNodeEvaluationParams& Params,
	FCameraNodeEvaluationResult& OutResult)
{
	using namespace WhiteBox::InputAxisBinding::Private;

	const UWhiteBoxInputAxisBinding2DCameraNode* AxisBindingNode =
		GetCameraNodeAs<UWhiteBoxInputAxisBinding2DCameraNode>();
	if (!AxisBindingNode)
	{
		return;
	}

	FVector2d HighestValue(FVector2d::ZeroVector);
	double HighestSquaredLength = 0.0;

	for (FEnhancedInputActionValueBinding* AxisValueBinding : AxisValueBindings)
	{
		if (!AxisValueBinding)
		{
			continue;
		}

		const FVector2d Value = AxisValueBinding->GetValue().Get<FVector2D>();
		const double ValueSquaredLength = Value.SquaredLength();
		if (ValueSquaredLength > HighestSquaredLength)
		{
			HighestValue = Value;
			HighestSquaredLength = ValueSquaredLength;
		}
	}

	const FVector2d Multiplier = MultiplierReader.Get(OutResult.VariableTable);
	FVector2d FrameInput(HighestValue.X * Multiplier.X, HighestValue.Y * Multiplier.Y);

	if (RevertAxisXReader.Get(OutResult.VariableTable))
	{
		FrameInput.X = -FrameInput.X;
	}
	if (RevertAxisYReader.Get(OutResult.VariableTable))
	{
		FrameInput.Y = -FrameInput.Y;
	}

	if (AxisBindingNode->InputSlotParameters.bIsAccumulated)
	{
		InputValue += FrameInput;
	}
	else
	{
		InputValue = FrameInput;
	}

	InputValue.X = NormalizeAxisValue(AxisBindingNode->NormalizeX, InputValue.X);
	InputValue.Y = NormalizeAxisValue(AxisBindingNode->NormalizeY, InputValue.Y);

	InputValue.X = ClampAxisValue(AxisBindingNode->ClampX, InputValue.X);
	InputValue.Y = ClampAxisValue(AxisBindingNode->ClampY, InputValue.Y);
}

} // namespace UE::Cameras

UWhiteBoxInputAxisBinding2DCameraNode::UWhiteBoxInputAxisBinding2DCameraNode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Multiplier = FVector2D(1, 1);
}

FCameraNodeEvaluatorPtr UWhiteBoxInputAxisBinding2DCameraNode::OnBuildEvaluator(
	FCameraNodeEvaluatorBuilder& Builder) const
{
	using namespace UE::Cameras;
	return Builder.BuildEvaluator<FWhiteBoxInputAxisBinding2DCameraNodeEvaluator>();
}
