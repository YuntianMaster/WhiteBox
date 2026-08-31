// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/Node/CaBoomOffsetInLock.h"

#include "Components/ActorComponent.h"
#include "Core/CameraEvaluationContext.h"
#include "Core/CameraNodeEvaluator.h"
#include "Core/CameraParameterReader.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CaBoomOffsetInLock)


namespace UE::Cameras
{
	class FCaBoomOffsetInLockEvaluator : public FCameraNodeEvaluator
	{
		UE_DECLARE_CAMERA_NODE_EVALUATOR(WHITEBOX_API, FCaBoomOffsetInLockEvaluator)

	protected:
		virtual void OnRun(const FCameraNodeEvaluationParams& Params, FCameraNodeEvaluationResult& OutResult) override;
		virtual void OnInitialize(const FCameraNodeEvaluatorInitializeParams& Params, FCameraNodeEvaluationResult& OutResult) override;
		FVector GetBoomArm(const FCameraNodeEvaluationResult& Result) const;

	private:
		TCameraParameterReader<float> MinPitchReader;
		TCameraParameterReader<float> MaxPitchReader;
		TCameraParameterReader<float> BoomZoomSpeedReader;
		TCameraParameterReader<float> CenterToEnemy;
		TCameraParameterReader<FVector3d> BoomOffsetReader;
		TCameraParameterReader<FVector3d> TargetLocationReader;
	};

	UE_DEFINE_CAMERA_NODE_EVALUATOR(FCaBoomOffsetInLockEvaluator)


	FVector FCaBoomOffsetInLockEvaluator::GetBoomArm(const FCameraNodeEvaluationResult& Result) const
	{
		const FVector3d CamLoc = Result.CameraPose.GetLocation();
		TArrayView<const FCameraRigJoint> Joints = Result.CameraRigJoints.GetJoints();

		if (Joints.Num() == 0)
		{
			return FVector::ZeroVector;
		}

		const FTransform3d& JointTransform = Joints.Last().Transform;
		const FVector3d RealBoomOffset = JointTransform.InverseTransformPosition(CamLoc);

		return FVector(RealBoomOffset.X, RealBoomOffset.Y, RealBoomOffset.Z);
	}

	void FCaBoomOffsetInLockEvaluator::OnInitialize(const FCameraNodeEvaluatorInitializeParams& Params, FCameraNodeEvaluationResult& OutResult)
	{
		const UCaBoomOffsetInLock* BoomOffsetNode = GetCameraNodeAs<UCaBoomOffsetInLock>();
		if (!BoomOffsetNode)
		{
			return;
		}

		// 参数若被 Variable / Interface 驱动，.Value 可能是 0；default 在 Variable 上，需用 Reader。
		MinPitchReader.Initialize(BoomOffsetNode->MinPitch);
		MaxPitchReader.Initialize(BoomOffsetNode->MaxPitch);
		CenterToEnemy.Initialize(BoomOffsetNode->CenterToEnemy);
		BoomZoomSpeedReader.Initialize(BoomOffsetNode->BoomZoomSpeed);
		BoomOffsetReader.Initialize(BoomOffsetNode->BoomOffset);
		TargetLocationReader.Initialize(BoomOffsetNode->FocusTarget);

		//if (UVector3dCameraVariable* Var = BoomOffsetNode->BoomOffset.Variable)
		//{
		//	if (const FCameraNodeEvaluationResult* LastResult = Params.LastActiveCameraRigInfo.LastResult)
		//	{
		//		FVector3d InitialBoom;
		//		if (LastResult->VariableTable.TryGetValue(Var, InitialBoom))
		//		{
		//			OutResult.VariableTable.SetValue(Var, InitialBoom);

		//			UE_LOG(LogTemp, Warning, TEXT("CurrentBoom get : %s"), *InitialBoom.ToString());
		//		}
		//		else
		//		{
		//			// 上一台没写过这个 Variable 时，再从上一台 Result 的 Pose/Joints 反推
		//			OutResult.VariableTable.SetValue(Var, GetBoomArm(*LastResult));
		//			UE_LOG(LogTemp, Warning, TEXT("CurrentBoom fromlast  : %s"), *GetBoomArm(*LastResult).ToString());
		//		}
		//	}
		//}
	}

	void FCaBoomOffsetInLockEvaluator::OnRun(const FCameraNodeEvaluationParams& Params, FCameraNodeEvaluationResult& OutResult)
	{



		const UCaBoomOffsetInLock* BoomOffsetNode = GetCameraNodeAs<UCaBoomOffsetInLock>();
		if (!BoomOffsetNode)
		{
			UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockEvaluator::OnRun: BoomOffsetNode is null"));	
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

		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OwnerActor);
		if(!PlayerCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockEvaluator::OnRun: ContextOwner is not APlayerCharacter"));	
			return;
		}

		AActor* TargetActor = PlayerCharacter->TargetActor;
		if(!TargetActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockEvaluator::OnRun: TargetActor is null"));	
			return;
		}

		const float MaxPitch = MaxPitchReader.Get(OutResult.VariableTable);
		const float MinPitch = MinPitchReader.Get(OutResult.VariableTable);
		const float float_CenterToEnemy = CenterToEnemy.Get(OutResult.VariableTable);
		//UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockEvaluator::OnRun: CenterToEnemy : %f"), float_CenterToEnemy);

		float Margin = BoomOffsetNode->FramingSize;
		const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
		const FVector TargetLocation = TargetActor->GetActorLocation();
		const FVector FocusLocation = UKismetMathLibrary::VLerp(PlayerLocation, TargetLocation, UKismetMathLibrary::FClamp(float_CenterToEnemy, 0, 1));
		//将摄像机的焦点位置存储到变量表中，以便其他节点或系统可以访问和使用这个焦点位置。
		if (UVector3dCameraVariable* Var = BoomOffsetNode->FocusTarget.Variable)
		{
			OutResult.VariableTable.SetValue(Var, FocusLocation);
		}
		FRotator DesiredRot = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, FocusLocation);

		UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockEvaluator::OnRun: FocusLocation : %s"), *FocusLocation.ToString());

		bool bPitchAtLimit = DesiredRot.Pitch > MaxPitch ||
			DesiredRot.Pitch< MinPitch;

		if (bPitchAtLimit) {

			Margin *= BoomOffsetNode->PitchLimitMarginScale;
		}

		DesiredRot.Pitch = FMath::Clamp(DesiredRot.Pitch, MinPitch, MaxPitch);

		const float FieldOfView = OutResult.CameraPose.GetFieldOfView();
		float AspectRatio = 16.f / 9.f;
		if (APlayerController* PlayerController = Params.EvaluationContext->GetPlayerController()) {

			if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
			{
				if (LocalPlayer->ViewportClient) {

					FVector2D ViewportSize;
					LocalPlayer->ViewportClient->GetViewportSize(ViewportSize);
					if (ViewportSize.Y > 0) {

						AspectRatio = ViewportSize.X / ViewportSize.Y;
					}
				}
			}

		}

		auto BoomOffset = [&](float Lenght)->FVector
			{
				return FVector(-Lenght, BoomOffsetNode->BoomLatera, BoomOffsetNode->BoomHeight);
			};

		auto CameraPos = [&](float Lenght)->FVector
			{
				return PlayerLocation + DesiredRot.RotateVector(BoomOffset(Lenght));
			};  

		auto ForwardAfterFrame = [&](float Lenght)->FVector
			{
				FVector CameraLocation = CameraPos(Lenght); 
				FRotator ROT = UKismetMathLibrary::FindLookAtRotation(CameraLocation, FocusLocation);
				return ROT.Vector();
			};

		auto RightAfterFrame = [&](float Lenght)->FVector
			{
				FVector CameraLocation = CameraPos(Lenght);
				FRotator ROT = UKismetMathLibrary::FindLookAtRotation(CameraLocation, FocusLocation);
				return FRotationMatrix(ROT).GetScaledAxis(EAxis::Y);
			};

		auto UpAfterFrame = [&](float Lenght)->FVector
			{
				FVector CameraLocation = CameraPos(Lenght);
				FRotator ROT = UKismetMathLibrary::FindLookAtRotation(CameraLocation, FocusLocation);
				return FRotationMatrix(ROT).GetScaledAxis(EAxis::Z);
			};

		const float HalfV = FMath::DegreesToRadians(FieldOfView / 2);
		const float HalfH = FMath::Atan(FMath::Tan(HalfV) * AspectRatio);
		const float TanH = FMath::Tan(HalfH);
		const float TanV = FMath::Tan(HalfV);
		const float SafeH = TanH * Margin;
		const float SafeV = TanV * Margin;


		TArray<FVector,TInlineAllocator<8>> TargetPts;

		TargetPts.Add(PlayerLocation + FVector(0, 0, BoomOffsetNode->PlayerHalfHeight));
		TargetPts.Add(PlayerLocation - FVector(0, 0, BoomOffsetNode->PlayerHalfHeight));
		TargetPts.Add(TargetLocation + FVector(0, 0, BoomOffsetNode->EnemyHalfHeight));
		TargetPts.Add(TargetLocation - FVector(0, 0, BoomOffsetNode->EnemyHalfHeight));


		auto Fits = [&](float Length)->bool {

			FVector CamLoc = CameraPos(Length);

			auto Inside = [&](FVector Checkloc)->bool {
				const FVector Rel = Checkloc - CamLoc;
				const float Depth = FVector::DotProduct(Rel, ForwardAfterFrame(Length));
				if(Depth<=KINDA_SMALL_NUMBER)
				{
					return false;
				}
				const float r = FMath::Abs(FVector::DotProduct(Rel, RightAfterFrame(Length)));
				const float u = FMath::Abs(FVector::DotProduct(Rel, UpAfterFrame(Length)));

				return r <= Depth * SafeH && u <= Depth * SafeV;
				};


			for (const FVector& Pt : TargetPts) {

				if(!Inside(Pt))
				{
					return false;
				}

			}
			return true;
			};





		float Hi = BoomOffsetNode->MaxBoomLength;
		float Lo = BoomOffsetNode->MinBoomLength;
		

		for(int i = 0; i < 20; ++i)
		{
			const float Mid = 0.5f * (Lo + Hi);
			if(Fits(Mid))
			{
				Hi = Mid;
			}
			else
			{
				Lo = Mid;
			}
		}



		//获得当前Boom数值
		FVector CurrentBoom = GetBoomArm(OutResult);
		UE_LOG(LogTemp, Warning, TEXT("CurrentBoom: %s"), *CurrentBoom.ToString());
		//获得计算数值
		FVector FinalBoomOffset = FVector(-Hi, BoomOffsetNode->BoomLatera, BoomOffsetNode->BoomHeight);
		UE_LOG(LogTemp, Warning, TEXT("FinalBoomOffset: %s"), *FinalBoomOffset.ToString());
		float BoomZoomSpeed = BoomZoomSpeedReader.Get(OutResult.VariableTable);
		FinalBoomOffset = UKismetMathLibrary::VInterpTo_Constant(CurrentBoom, FinalBoomOffset, Params.DeltaTime, BoomZoomSpeed);
		if (UVector3dCameraVariable* Var = BoomOffsetNode->BoomOffset.Variable)
		{
			OutResult.VariableTable.SetValue(Var, FinalBoomOffset);
		}
	}

} // namespace UE::Cameras

FCameraNodeEvaluatorPtr UCaBoomOffsetInLock::OnBuildEvaluator(FCameraNodeEvaluatorBuilder& Builder) const
{
	using namespace UE::Cameras;
	return Builder.BuildEvaluator<FCaBoomOffsetInLockEvaluator>();
}

