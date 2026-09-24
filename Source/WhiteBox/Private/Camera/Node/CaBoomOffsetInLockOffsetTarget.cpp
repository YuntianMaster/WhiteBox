// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/Node/CaBoomOffsetInLockOffsetTarget.h"
#include "Components/ActorComponent.h"
#include "Core/CameraEvaluationContext.h"
#include "Core/CameraNodeEvaluator.h"
#include "Core/CameraParameterReader.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(CaBoomOffsetInLockOffsetTarget)

namespace UE::Cameras {
	class FCaBoomOffsetInLockOffsetTargetEvaluator : public FCameraNodeEvaluator
	{
		UE_DECLARE_CAMERA_NODE_EVALUATOR(WHITEBOX_API, FCaBoomOffsetInLockOffsetTargetEvaluator)

	protected:
		virtual void OnRun(const FCameraNodeEvaluationParams& Params, FCameraNodeEvaluationResult& OutResult) override;

		virtual void OnInitialize(const FCameraNodeEvaluatorInitializeParams& Params, FCameraNodeEvaluationResult& OutResult) override;
		
		FVector GetBoomArmOffset(const FCameraNodeEvaluationResult& Result) const;

		FRotator CaCamRot(AActor* PlayerOwner);

	private:
		TCameraParameterReader<float> MinPitchReader;
		TCameraParameterReader<float> MaxPitchReader;
		TCameraParameterReader<float> CenterToEnemy;
		TCameraParameterReader<float> BoomZoomSpeedReader;
		TCameraParameterReader<FVector3d> BoomOffsetReader;
		TCameraParameterReader<FVector3d> TargetLocationReader;

		FVector InitialActorLoc;
		FVector InitialTargetLoc;
		FTransform InitialPlayerTran;
		FRotator InitialCameraRot;

	};

	UE_DEFINE_CAMERA_NODE_EVALUATOR(FCaBoomOffsetInLockOffsetTargetEvaluator)



	void FCaBoomOffsetInLockOffsetTargetEvaluator::OnRun(const FCameraNodeEvaluationParams& Params, FCameraNodeEvaluationResult& OutResult)
	{
		const UCaBoomOffsetInLockOffsetTarget* CaBoomOffsetInLockOffsetTargetNode = GetCameraNodeAs<UCaBoomOffsetInLockOffsetTarget>();
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
		if (!PlayerCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockOffsetTargetEvaluator::OnRun: ContextOwner is not APlayerCharacter"));
			return;
		}
	
		AActor* TargetActor = PlayerCharacter->TargetActor;
		if(!TargetActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockOffsetTargetEvaluator::OnRun: TargetActor is not Exists"));
			return;
		}

		PlayerCharacter->bUseControllerRotationYaw = false;

		const float MaxPitch = MaxPitchReader.Get(OutResult.VariableTable);
		const float MinPitch = MinPitchReader.Get(OutResult.VariableTable);
		const float float_CenterToEnemy = CenterToEnemy.Get(OutResult.VariableTable);
		float Margin = CaBoomOffsetInLockOffsetTargetNode->FramingSize;
		const FVector CurrentTargetLoc = TargetActor->GetActorLocation();

		
	

	

		const FVector PlayerLocation = OwnerActor->GetActorLocation();
	

		
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

		FTransform PlayerTrans = OwnerActor->GetActorTransform();

		auto BoomOffset = [&](float Length)->FVector {
			return FVector(-Length, CaBoomOffsetInLockOffsetTargetNode->BoomLatera, CaBoomOffsetInLockOffsetTargetNode->BoomHeight);
			};


		auto CameraPos = [&](float Length)->FVector {

			return PlayerTrans.TransformPosition(BoomOffset(Length));
				
				
			};

		auto CameraForward = [&](float Length)->FVector {
			FRotator Rot = UKismetMathLibrary::FindLookAtRotation(CameraPos(Length), TargetActor->GetActorLocation());
			return Rot.Vector();

			};

		auto CameraRight = [&](float Length)->FVector {
			FRotator Rot = UKismetMathLibrary::FindLookAtRotation(CameraPos(Length), TargetActor->GetActorLocation());
			return FRotationMatrix(Rot).GetScaledAxis(EAxis::Y);
			};

		auto CameraUp = [&](float Length)->FVector {
			FRotator Rot = UKismetMathLibrary::FindLookAtRotation(CameraPos(Length), TargetActor->GetActorLocation());
			return FRotationMatrix(Rot).GetScaledAxis(EAxis::Z);
			};
		const float HalfV = FMath::DegreesToRadians(FieldOfView / 2);
		const float HalfH = FMath::Atan(FMath::Tan(HalfV) * AspectRatio);
		const float TanH = FMath::Tan(HalfH);
		const float TanV = FMath::Tan(HalfV);
		const float SafeH = TanH * Margin;
		const float SafeV = TanV * Margin;
	

		TArray<FVector, TInlineAllocator<8>> TargetPts;

		TargetPts.Add(PlayerLocation + FVector(0, 0, CaBoomOffsetInLockOffsetTargetNode->PlayerHalfHeight));
		TargetPts.Add(PlayerLocation - FVector(0, 0, CaBoomOffsetInLockOffsetTargetNode->PlayerHalfHeight));
		TargetPts.Add(CurrentTargetLoc + FVector(0, 0, CaBoomOffsetInLockOffsetTargetNode->EnemyHalfHeight));
		TargetPts.Add(CurrentTargetLoc - FVector(0, 0, CaBoomOffsetInLockOffsetTargetNode->EnemyHalfHeight));

		auto Fits = [&](float Length)->bool {

			FVector CamLoc = CameraPos(Length);
			auto Inside = [&](FVector Checkloc)->bool {
				const FVector Rel = Checkloc - CamLoc;
				const float Depth = FVector::DotProduct(Rel, CameraForward(Length));
				if (Depth <= KINDA_SMALL_NUMBER)
				{
					return false;
				}
				const float r = FMath::Abs(FVector::DotProduct(Rel, CameraRight(Length)));
				const float u = FMath::Abs(FVector::DotProduct(Rel, CameraUp(Length)));

				return r <= Depth * SafeH && u <= Depth * SafeV;

				};
				for (const FVector& Pt : TargetPts) {

					if (!Inside(Pt))
					{
						return false;
					}

				}
				return true;

			};

		float Hi = CaBoomOffsetInLockOffsetTargetNode->MaxBoomLength;
		float Lo = CaBoomOffsetInLockOffsetTargetNode->MinBoomLength;
		for (int i = 0; i < 20; ++i)
		{
			const float Mid = 0.5f * (Lo + Hi);
			if (Fits(Mid))
			{
				Hi = Mid;
			}
			else
			{
				Lo = Mid;
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("Hi:%f"), Hi);
		FRotator PlayerR = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetActor->GetActorLocation());
		FVector CamP = PlayerTrans.TransformPosition(FVector(-Hi, CaBoomOffsetInLockOffsetTargetNode->BoomLatera, CaBoomOffsetInLockOffsetTargetNode->BoomHeight));
		FRotator CamR = UKismetMathLibrary::FindLookAtRotation(CamP, TargetActor->GetActorLocation());
		
		
		OutResult.CameraPose.SetLocation(CamP);
		OutResult.CameraPose.SetRotation(CamR);
		PlayerR.Pitch = 0;
		PlayerR.Roll = 0;
		OwnerActor->SetActorRotation(PlayerR);

	}

	void FCaBoomOffsetInLockOffsetTargetEvaluator::OnInitialize(const FCameraNodeEvaluatorInitializeParams& Params, FCameraNodeEvaluationResult& OutResult)
	{
		const UCaBoomOffsetInLockOffsetTarget* CaBoomOffsetInLockOffsetTargetNode = GetCameraNodeAs<UCaBoomOffsetInLockOffsetTarget>();
		
		BoomOffsetReader.Initialize(CaBoomOffsetInLockOffsetTargetNode->BoomOffset);
		if (UVector3dCameraVariable* Var = CaBoomOffsetInLockOffsetTargetNode->BoomOffset.Variable)
		{
			if (const FCameraNodeEvaluationResult* LastResult = Params.LastActiveCameraRigInfo.LastResult)
			{

				InitialCameraRot = LastResult->CameraPose.GetRotation();
				FVector3d InitialBoom;
				if (LastResult->VariableTable.TryGetValue(Var, InitialBoom))
				{
					OutResult.VariableTable.SetValue(Var, InitialBoom);

					UE_LOG(LogTemp, Warning, TEXT("CurrentBoom get : %s"), *InitialBoom.ToString());
				}
				else
				{
					// ?????????? Variable ?????????? Result ?? Pose/Joints ????
					OutResult.VariableTable.SetValue(Var, GetBoomArmOffset(*LastResult));
					UE_LOG(LogTemp, Warning, TEXT("CurrentBoom fromlast  : %s"), *GetBoomArmOffset(*LastResult).ToString());
				}
			}
		}


		MinPitchReader.Initialize(CaBoomOffsetInLockOffsetTargetNode->MinPitch);
		MaxPitchReader.Initialize(CaBoomOffsetInLockOffsetTargetNode->MaxPitch);
		CenterToEnemy.Initialize(CaBoomOffsetInLockOffsetTargetNode->CenterToEnemy);
		BoomOffsetReader.Initialize(CaBoomOffsetInLockOffsetTargetNode->BoomOffset);
		BoomZoomSpeedReader.Initialize(CaBoomOffsetInLockOffsetTargetNode->BoomZoomSpeed);
		TargetLocationReader.Initialize(CaBoomOffsetInLockOffsetTargetNode->FocusTarget);

		if (!CaBoomOffsetInLockOffsetTargetNode)
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

		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OwnerActor);
		if (!PlayerCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockOffsetTargetEvaluator::OnInitialize: ContextOwner is not APlayerCharacter"));
			return;
		}


		





		if (AActor* TargetActor = PlayerCharacter->TargetActor)
		{
			InitialTargetLoc = TargetActor->GetActorLocation();
			InitialPlayerTran = TargetActor->GetActorTransform();
			UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockOffsetTargetEvaluator::OnInitialize: InitialTargetLoc: %s"), *InitialTargetLoc.ToString());

			FVector PlayerLocation = PlayerCharacter->GetActorLocation();
			FTransform PlayerTrans = PlayerCharacter->GetTransform();
			FRotator PlayerR = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetActor->GetActorLocation());
			FVector CamP = PlayerTrans.TransformPosition(FVector(CaBoomOffsetInLockOffsetTargetNode->MinBoomLength, CaBoomOffsetInLockOffsetTargetNode->BoomLatera, CaBoomOffsetInLockOffsetTargetNode->BoomHeight));
			FRotator CamR = UKismetMathLibrary::FindLookAtRotation(CamP, TargetActor->GetActorLocation());


			if(CaBoomOffsetInLockOffsetTargetNode->bIsDebug)
			{

				UKismetSystemLibrary::DrawDebugSphere(
					TargetActor->GetWorld(),
					InitialActorLoc,
					20.f,
					12,
					FLinearColor::Green,
					-1,
					5.f
				);
			}



			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockOffsetTargetEvaluator::OnInitialize: TargetActor is not Exist"));


		
	}

	FVector FCaBoomOffsetInLockOffsetTargetEvaluator::GetBoomArmOffset(const FCameraNodeEvaluationResult& Result) const
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

	

}


FCameraNodeEvaluatorPtr UCaBoomOffsetInLockOffsetTarget::OnBuildEvaluator(FCameraNodeEvaluatorBuilder& Builder) const
{
	using namespace UE::Cameras;
	return Builder.BuildEvaluator<FCaBoomOffsetInLockOffsetTargetEvaluator>();
}
