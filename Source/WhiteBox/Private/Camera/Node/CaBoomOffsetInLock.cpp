// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/Node/CaBoomOffsetInLock.h"
#include "Components/ActorComponent.h"
#include "Core/CameraEvaluationContext.h"
#include "Core/CameraNodeEvaluator.h"
#include "Core/CameraParameterReader.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/CamerManagerComponent.h"
#include "Enum/E_Camera.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/PlayerCharacter.h"
#include "CanvasItem.h"
#include "Debug/DebugDrawService.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Math/CriticalDamper.h"
#include "Structure/FCameraArmStruct.h"
#include "GameFramework/PlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CaBoomOffsetInLock)



struct FLocalCriticalDamper
{
	float W0 = 10.f;
	float X0 = 0.f;
	float X0Derivative = 0.f;
	void Reset(float InX0, float InX0Derivative)
	{
		X0 = InX0;
		X0Derivative = InX0Derivative;
	}
	void SetW0(float InW0) { W0 = InW0; }
	float Update(float X, float DeltaTime)
	{
		const float ForcedMovement = X - X0;
		if (W0 > 0.f && DeltaTime > 0.f)
		{
			const float Dv = (ForcedMovement != 0.f) ? (ForcedMovement / DeltaTime) : 0.f;
			const float P0 = 2.f * Dv / W0;
			const float P1 = -Dv;
			const float Exp = FMath::Exp(-W0 * DeltaTime);
			const float A = X0 - P0;
			const float B = X0Derivative + W0 * A - P1;
			const float Xt = (A + B * DeltaTime) * Exp + P0 + P1 * DeltaTime;
			const float XtDeriv = (-W0 * A + B - W0 * B * DeltaTime) * Exp + P1;
			X0 = Xt + ForcedMovement;
			X0Derivative = XtDeriv;
			if (FMath::Abs(X0) <= 1.e-5f && FMath::Abs(X0Derivative) <= 1.e-5f)
			{
				X0 = 0.f;
				X0Derivative = 0.f;
			}
		}
		else
		{
			X0 = 0.f;
			X0Derivative = 0.f;
		}
		return X0;
	}
};

namespace CaBoomOffsetInLockDebug
{
	struct FDeadZoneScreenDebug
	{
		FVector2D IdealUV = FVector2D(0.5f, 0.5f);
		FVector2D CurrentUV = FVector2D(0.5f, 0.5f);
		float DeadHalf = 0.2f;
		float SoftHalf = 0.3f;
		bool bActive = false;
	};

	static FDeadZoneScreenDebug GDebugState;
	static bool GDebugDrawRegistered = false;

	static void DrawDeadZoneScreenDebug(UCanvas* Canvas, APlayerController* /*PC*/)
	{
		if (!GDebugState.bActive || !Canvas)
		{
			return;
		}

		const float SizeX = Canvas->ClipX;
		const float SizeY = Canvas->ClipY;
		if (SizeX <= KINDA_SMALL_NUMBER || SizeY <= KINDA_SMALL_NUMBER)
		{
			return;
		}

		auto UVToPixel = [&](const FVector2D& UV) -> FVector2D
		{
			return FVector2D(UV.X * SizeX, (1.f - UV.Y) * SizeY);
		};

		auto DrawLine2D = [&](const FVector2D& A, const FVector2D& B, const FLinearColor& Color)
		{
			FCanvasLineItem Line(A, B);
			Line.SetColor(Color);
			Line.LineThickness = 1.f;
			Canvas->Canvas->DrawItem(Line);
		};

		auto DrawRect2D = [&](const FVector2D& Center, float Half, const FLinearColor& Color)
		{
			const FVector2D TL = UVToPixel(FVector2D(Center.X - Half, Center.Y + Half));
			const FVector2D TR = UVToPixel(FVector2D(Center.X + Half, Center.Y + Half));
			const FVector2D BR = UVToPixel(FVector2D(Center.X + Half, Center.Y - Half));
			const FVector2D BL = UVToPixel(FVector2D(Center.X - Half, Center.Y - Half));
			DrawLine2D(TL, TR, Color);
			DrawLine2D(TR, BR, Color);
			DrawLine2D(BR, BL, Color);
			DrawLine2D(BL, TL, Color);
		};

		auto DrawPoint2D = [&](const FVector2D& UV, const FLinearColor& Color, float Size)
		{
			const FVector2D P = UVToPixel(UV);
			const float Half = Size * 0.5f;
			FCanvasTileItem Tile(FVector2D(P.X - Half, P.Y - Half), FVector2D(Size, Size), Color);
			Tile.BlendMode = SE_BLEND_Translucent;
			Canvas->Canvas->DrawItem(Tile);
		};

		const FVector2D Center = GDebugState.CurrentUV;
		DrawRect2D(Center, GDebugState.DeadHalf, FLinearColor::Green);
		DrawRect2D(Center, GDebugState.SoftHalf, FLinearColor::Red);
		DrawPoint2D(GDebugState.IdealUV, FLinearColor::Green, 8.f);
		DrawPoint2D(GDebugState.CurrentUV, FLinearColor::Red, 8.f);
	}
}

namespace UE::Cameras
{
	class FCaBoomOffsetInLockEvaluator : public FCameraNodeEvaluator
	{
		UE_DECLARE_CAMERA_NODE_EVALUATOR(WHITEBOX_API, FCaBoomOffsetInLockEvaluator)
	public:
		~FCaBoomOffsetInLockEvaluator()
		{
			UnbindLockModeDelegate();
			CaBoomOffsetInLockDebug::GDebugState.bActive = false;
		}

	protected:
		virtual void OnRun(const FCameraNodeEvaluationParams& Params, FCameraNodeEvaluationResult& OutResult) override;
		virtual void OnInitialize(const FCameraNodeEvaluatorInitializeParams& Params, FCameraNodeEvaluationResult& OutResult) override;
		FVector GetBoomArm(const FCameraNodeEvaluationResult& Result) const;
		void OnCameraLockModeChanged(E_LockCameraMode CameraLockMode);
		void OnCameraArmValueChanged(FCameraArmStruct BoomArmValue);
		void UnbindLockModeDelegate();
		void CaculateFocusLocation(E_LockCameraMode CameraLockMode,const FCameraNodeEvaluationParams& Params, FCameraNodeEvaluationResult& OutResult);
		void DeadZoneHandle(const FCameraNodeEvaluationParams& Params, FCameraNodeEvaluationResult& OutResult, FVector TableLocation, FVector CALocation);
		void CABoomArm(const FCameraNodeEvaluationParams& Params, FCameraNodeEvaluationResult& OutResult);
		float GetTargetDistance(const FCameraNodeEvaluationParams& Params, FCameraNodeEvaluationResult& OutResult);
		bool CaculateCameraRotate(const FVector& PlayerLoc,const FVector& TargetLoc,const FVector& BoomArm, FRotator& OutRot);

	private:
		UCamerManagerComponent* UMC;
		E_LockCameraMode CurrentCameraLockMode;
		E_LockCameraMode LastCameraLockCurrentMode;
		TCameraParameterReader<float> MinPitchReader;
		TCameraParameterReader<float> MaxPitchReader;
		TCameraParameterReader<float> BoomZoomSpeedReader;
		TCameraParameterReader<float> CenterToEnemy;
		TCameraParameterReader<FVector3d> BoomOffsetReader;
		TCameraParameterReader<FVector3d> FocusLocationReader;
		FLocalCriticalDamper ReframeDamper;
	
		//进入TargetLockOffset时的坐标信息
		FVector EnterLockOffsetFocusLocation;
		FDelegateHandle LockModeChangedHandle;
		FDelegateHandle BoomArmValueChangedHandle;

		//BoomArm
		FCameraArmStruct CurrentArmValue;

		//冻结参数
		float LastOrbitYaw = 0.f;
		bool bOrbitYawInitialized = false;
		FVector LastPlayerLoc;
		FVector LastFocusLoc;
		FVector	LastBoom;

		FRotator LastCameraRot;
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

	void FCaBoomOffsetInLockEvaluator::OnCameraLockModeChanged(E_LockCameraMode CameraLockMode)
	{

		CurrentCameraLockMode = CameraLockMode;
		//UE_LOG(LogTemp, Error, TEXT("CurrentCameraLockMode: %s"), *UEnum::GetValueAsString(CurrentCameraLockMode));

	}

	void FCaBoomOffsetInLockEvaluator::OnCameraArmValueChanged(FCameraArmStruct BoomArmValue)
	{
		CurrentArmValue = BoomArmValue;
		//UE_LOG(LogTemp, Error, TEXT("FCaBoomOffsetInLockEvaluator::CurrentCameraArmValue: %f"), BoomArmValue.BoomHeight);
	}

	void FCaBoomOffsetInLockEvaluator::UnbindLockModeDelegate()
	{
		if (UMC && LockModeChangedHandle.IsValid())
		{
			UMC->FOnCameraLockModeChangeDelegate.Remove(LockModeChangedHandle);
			LockModeChangedHandle.Reset();
		}

		if (UMC && BoomArmValueChangedHandle.IsValid())
		{
			UMC->FOnCameraBoomArmValueChangeDelegate.Remove(BoomArmValueChangedHandle);
			BoomArmValueChangedHandle.Reset();
		}
		
	}

	void FCaBoomOffsetInLockEvaluator::CaculateFocusLocation(E_LockCameraMode CameraLockMode, const FCameraNodeEvaluationParams& Params, FCameraNodeEvaluationResult& OutResult)
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
			if (!PlayerCharacter)
			{
				UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockEvaluator::OnRun: ContextOwner is not APlayerCharacter"));
				return;
			}

			AActor* TargetActor = PlayerCharacter->TargetActor;
			if (!TargetActor)
			{
				UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockEvaluator::OnRun: TargetActor is null"));
				return;
			}
		
			const float float_CenterToEnemy = CenterToEnemy.Get(OutResult.VariableTable);
			const FVector TargetLocation = TargetActor->GetActorLocation();
			const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
			FVector CAFocusLocation = FVector::ZeroVector;
		switch (CurrentCameraLockMode)
		{
		case LockTarget:
		{
			//UE_LOG(LogTemp, Error, TEXT(" FCaBoomOffsetInLockEvaluator::CaculateFocusLocation LockTarget: %s"), *UEnum::GetValueAsString(CurrentCameraLockMode));

			CAFocusLocation = UKismetMathLibrary::VLerp(PlayerLocation, TargetLocation, UKismetMathLibrary::FClamp(float_CenterToEnemy, 0, 1));
			EnterLockOffsetFocusLocation = CAFocusLocation;
			break;
		}
		case LockOffsetTarget:
		{
		/*	UE_LOG(LogTemp, Error, TEXT(" FCaBoomOffsetInLockEvaluator::CaculateFocusLocation LockOffsetTarget: %s"), *UEnum::GetValueAsString(CurrentCameraLockMode));*/

			// 当前相机姿态（Gameplay Cameras）
			const FVector CamLoc = OutResult.CameraPose.GetLocation();
			const FRotator CamRot = OutResult.CameraPose.GetRotation();
			const FTransform CamTM(CamRot, CamLoc);
			// 进入 Offset 时的焦点、当前敌人 → 相机空间
			const FVector EnterLocal = CamTM.InverseTransformPosition(EnterLockOffsetFocusLocation);
			const FVector EnemyLocal = CamTM.InverseTransformPosition(TargetLocation);
			// X、Z 跟敌人；Y（横向）保持进入时的值
			const FVector NewLocal(EnemyLocal.X, EnterLocal.Y, EnemyLocal.Z);
			CAFocusLocation = CamTM.TransformPosition(NewLocal);
			break;
		}
		default:
			break;
		}


		const FVector TableFocusLocation = FocusLocationReader.Get(OutResult.VariableTable);

		/*CAFocusLocation = UKismetMathLibrary::VInterpTo_Constant(TableFocusLocation, CAFocusLocation, Params.DeltaTime, BoomZoomSpeedReader.Get(OutResult.VariableTable));

		UE_LOG(LogTemp, Error, TEXT(" FCaBoomOffsetInLockEvaluator::CaculateFocusLocation LockOffsetTarget: %s"), *CAFocusLocation.ToString());
		if (UVector3dCameraVariable* Var = BoomOffsetNode->FocusTarget.Variable)
		{
			OutResult.VariableTable.SetValue(Var, CAFocusLocation);
		}*/


		DeadZoneHandle(Params,OutResult, TableFocusLocation, CAFocusLocation);

		//UE_LOG(LogTemp, Error, TEXT(" FCaBoomOffsetInLockEvaluator::CAFocusLocation : %s"), *CAFocusLocation.ToString());
		//UE_LOG(LogTemp, Error, TEXT(" FCaBoomOffsetInLockEvaluator::TableFocusLocation : %s"), *TableFocusLocation.ToString());



	}

	void FCaBoomOffsetInLockEvaluator::DeadZoneHandle(const FCameraNodeEvaluationParams& Params, FCameraNodeEvaluationResult& OutResult,FVector TableLocation,FVector CALocation)
	{
		const FVector CamLoc = OutResult.CameraPose.GetLocation();
		const FRotator CamRot = OutResult.CameraPose.GetRotation();
		const float FOVDeg = OutResult.CameraPose.GetFieldOfView();
		float Aspect = 16.f / 9.f;
		if (APlayerController* PC = Params.EvaluationContext->GetPlayerController())
		{
			int32 SizeX = 0, SizeY = 0;
			PC->GetViewportSize(SizeX, SizeY);
			if (SizeY > 0)
			{
				Aspect = static_cast<float>(SizeX) / static_cast<float>(SizeY);
			}
		}

		const UCaBoomOffsetInLock* BoomOffsetNode = GetCameraNodeAs<UCaBoomOffsetInLock>();

		auto ProjectToScreen = [&](const FVector& WorldLocation, FVector2D& OutUV, bool& bInFront) -> FVector
			{
				const float HalfV = FMath::DegreesToRadians(FOVDeg * 0.5f);
				const float HalfH = FMath::Atan(FMath::Tan(HalfV) * Aspect);

				const FVector Direction = WorldLocation - CamLoc;
				
				const FVector F = CamRot.Vector();
				const FVector R = FRotationMatrix(CamRot).GetScaledAxis(EAxis::Y);
				const FVector U = FRotationMatrix(CamRot).GetScaledAxis(EAxis::Z);

				const float ScreenDepth = FVector::DotProduct(Direction, F);
				const float ScreenWeight = ScreenDepth * FMath::Tan(HalfH);
				const float ScreenHeight = ScreenDepth * FMath::Tan(HalfV);

				
				const float FVectorRightDist = FVector::DotProduct(Direction, R);
				const float FVectorUpDist = FVector::DotProduct(Direction, U);

				bInFront = ScreenDepth > KINDA_SMALL_NUMBER;;
				if (!bInFront)
				{
					OutUV = FVector2D::ZeroVector;
					return FVector::ZeroVector;
				}


				OutUV.X = 0.5f + FVectorRightDist / (2 * ScreenWeight);
				OutUV.Y = 0.5f + FVectorUpDist / (2 * ScreenHeight);


				const float Xoffset = (OutUV.X - 0.5f) * 2 * ScreenWeight;
				const float Yoffset = (OutUV.Y - 0.5F) * 2 * ScreenHeight;

				return CamLoc + Xoffset * R + Yoffset * U;

			
			};

	


		//CurrentUV是table中数值，也是摄像机先阶段看向的位置
		//CalUV是当前目标的位置
		//只有目标超过Deadzone/SoftZone时，才会移动摄像机
		FVector2D CalUV, CurrentUV;
		

		bool bIdealInFront, bCurrentInFront;
		ProjectToScreen(CALocation, CalUV, bIdealInFront);
		const FVector2D ScreenCenter = BoomOffsetNode->ScreenCenter;
		ProjectToScreen(TableLocation, CurrentUV, bCurrentInFront);



		//屏幕投射计算
	/*	APlayerController* PC = Params.EvaluationContext->GetPlayerController();
		FVector2D ScreenPos;
		int32 SizeX = 0, SizeY = 0;
		bool bHit = PC->ProjectWorldLocationToScreen(CALocation, ScreenPos);
		PC->GetViewportSize(SizeX, SizeY);
		FVector2D Normalized(ScreenPos.X / SizeX, ScreenPos.Y / SizeY);

		CalUV = Normalized;*/
		//屏幕投射

		
		//UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockEvaluator::OnRun: ScreenCenter:%s"), *ScreenCenter.ToString());
		/*UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockEvaluator::OnRun: CalUV:%s"), *CalUV.ToString());*/
		//const FVector2D DeltaUV = CurrentUV - CalUV;
		
		FVector CAFocusLocation = TableLocation;
		//Deadzone SOftZone回归计算

		auto InRect = [](const FVector2D& P, float Half)
			{
				return FMath::Abs(P.X - 0.5f) <= Half
					&& FMath::Abs(P.Y - 0.5f) <= Half;
			};

		const float DeadHalf = BoomOffsetNode->DeadHalf;
		const float SoftHalf = BoomOffsetNode->SoftHalf;
		const bool bInDead = InRect(CalUV, DeadHalf);
		const bool bInSoft = InRect(CalUV, SoftHalf);


		if (bInDead || GetTargetDistance(Params,OutResult) < BoomOffsetNode->DeadZoneDistanceThreshold)
		{
			if (UVector3dCameraVariable* Var = BoomOffsetNode->FocusTarget.Variable)
			{
				OutResult.VariableTable.SetValue(Var, CAFocusLocation);
			}
			return;
		}
		//UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockEvaluator::OnRun: CalUV:%s"), *CalUV.ToString());
		

		const float UnlockRadius = BoomOffsetNode->UnlockRadius;           // UE 默认 ReframeUnlockRadius
		const float W0 = BoomOffsetNode->W0;

		static bool bIsReframing = false;

		if (BoomOffsetNode->bIsDebug && GEngine && GEngine->GameViewport)
		{
			using namespace CaBoomOffsetInLockDebug;

			GDebugState.IdealUV = CalUV;
			GDebugState.CurrentUV = ScreenCenter;
			GDebugState.DeadHalf = BoomOffsetNode->DeadHalf;
			GDebugState.SoftHalf = BoomOffsetNode->SoftHalf;
			GDebugState.bActive = true;

			if (!GDebugDrawRegistered)
			{
				GDebugDrawRegistered = true;
				UDebugDrawService::Register(
					TEXT("Game"),
					FDebugDrawDelegate::CreateStatic(&DrawDeadZoneScreenDebug));
			}
		}
		else
		{
			CaBoomOffsetInLockDebug::GDebugState.bActive = false;
		}

		if (!bInSoft)
		{
			bIsReframing = true;           // Hard：强制 reframing
		}
		else if (!bInDead)
		{
			bIsReframing = true;           // Soft：温和 reframing
		}
		else if (bIsReframing)
		{
			// 回到 Dead 后，收到 Unlock 半径才停（和 UE 一样）
			const float DistIdeal = FVector2D::Distance(
				FVector2D(ScreenCenter.X, (ScreenCenter.Y - 0.5f) / Aspect + 0.5f),
				FVector2D(CalUV.X, (CalUV.Y - 0.5f) / Aspect + 0.5f));
			if (DistIdeal <= UnlockRadius + KINDA_SMALL_NUMBER)
			{
				bIsReframing = false;
			}
		}

		if (bIsReframing)
		{
			FVector2D EffectiveScreen = CalUV;
			// Hard：先瞬间贴到 Soft 边（沿 Ideal→当前 方向）
			if (!bInSoft)
			{
				const FVector2D Diagonal = ScreenCenter - CalUV;
				const float Ax = FMath::Abs(Diagonal.X) > KINDA_SMALL_NUMBER
					? SoftHalf / FMath::Abs(Diagonal.X) : BIG_NUMBER;
				const float Ay = FMath::Abs(Diagonal.Y) > KINDA_SMALL_NUMBER
					? SoftHalf / FMath::Abs(Diagonal.Y) : BIG_NUMBER;
				EffectiveScreen = ScreenCenter + Diagonal * FMath::Min(Ax, Ay);
				
			}
			FVector2D IdealToTarget = EffectiveScreen - ScreenCenter;
			//UE_LOG(LogTemp, Warning, TEXT("IdealToTarget: %s"), *IdealToTarget.ToString());
			double DistanceToGo = IdealToTarget.Size();


			if (DistanceToGo > KINDA_SMALL_NUMBER)
			{
				ReframeDamper.SetW0(W0);
				const double DampingDistanceToGo = FMath::Max(0.0, DistanceToGo - UnlockRadius);
				// Update(当前距离)：向 0 收敛，返回新的「剩余距离」
				const double NewDamped = ReframeDamper.Update(DampingDistanceToGo, Params.DeltaTime);
				const double NewDistanceToGo = NewDamped + UnlockRadius;

				// --- 适配你们「改 Table 焦点」管线 ---
				// 屏幕上希望 CA 从 ScreenTarget 移到 DesiredScreen：
				// 用同一比例把 Table 往 CA 推（近似 UE 的 reframing 量）
				const double OldD = FMath::Max(DistanceToGo, UE_DOUBLE_SMALL_NUMBER);
				const double Alpha = FMath::Clamp(1.0 - (NewDistanceToGo / OldD), 0.0, 1.0);
				CAFocusLocation = FMath::Lerp(TableLocation, CALocation, Alpha);

				/*UE_LOG(LogTemp, Warning, TEXT("TableLocation: %s"), *TableLocation.ToString());
				UE_LOG(LogTemp, Warning, TEXT("CALocation: %s"), *CALocation.ToString());
				UE_LOG(LogTemp, Warning, TEXT("CAFocusLocation: %s"), *CAFocusLocation.ToString());*/

				if (UVector3dCameraVariable* Var = BoomOffsetNode->FocusTarget.Variable)
				{
					OutResult.VariableTable.SetValue(Var, CAFocusLocation);
				}
				// 若要更贴 UE：这里应改 CameraPose 的 Yaw/Pitch（Panning），而不是 Lerp 焦点
			}

		}

		/*if (FMath::Abs(DeltaUV.X) > BoomOffsetNode->DeadHalf || FMath::Abs(DeltaUV.Y) > BoomOffsetNode->DeadHalf)
		{
			CAFocusLocation = UKismetMathLibrary::VInterpTo_Constant(
				TableLocation, CALocation, Params.DeltaTime, BoomOffsetNode->DeadZoneRecoverySpeed);

			if(FMath::Abs(DeltaUV.X)> BoomOffsetNode->SoftHalf || FMath::Abs(DeltaUV.Y) > BoomOffsetNode->SoftHalf)
			{
				CAFocusLocation = UKismetMathLibrary::VInterpTo_Constant(
					TableLocation, CALocation, Params.DeltaTime, BoomOffsetNode->SoftZoneRecoverySpeed);
			}
		}
		
		if (UVector3dCameraVariable* Var = BoomOffsetNode->FocusTarget.Variable)
		{
			OutResult.VariableTable.SetValue(Var, CAFocusLocation);
		}*/




	}

	void FCaBoomOffsetInLockEvaluator::CABoomArm(const FCameraNodeEvaluationParams& Params, FCameraNodeEvaluationResult& OutResult)
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
		if (!PlayerCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockEvaluator::OnRun: ContextOwner is not APlayerCharacter"));
			return;
		}

		AActor* TargetActor = PlayerCharacter->TargetActor;
		if (!TargetActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockEvaluator::OnRun: TargetActor is null"));
			return;
		}

		CaculateFocusLocation(CurrentCameraLockMode, Params, OutResult);

		const float MaxPitch = MaxPitchReader.Get(OutResult.VariableTable);
		const float MinPitch = MinPitchReader.Get(OutResult.VariableTable);
		//const float float_CenterToEnemy = CenterToEnemy.Get(OutResult.VariableTable);
		////UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockEvaluator::OnRun: CenterToEnemy : %f"), float_CenterToEnemy);

		float Margin = BoomOffsetNode->FramingSize;
		
		//FVector FocusLocation = UKismetMathLibrary::VLerp(PlayerLocation, TargetLocation, UKismetMathLibrary::FClamp(float_CenterToEnemy, 0, 1));
	


		const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
		const FVector TargetLocation = TargetActor->GetActorLocation();
		const FVector FocusLocation = FocusLocationReader.Get(OutResult.VariableTable);
		FRotator DesiredRot = UKismetMathLibrary::FindLookAtRotation(OutResult.CameraPose.GetLocation(), FocusLocation);

		const FVector F = PlayerCharacter->GetActorRotation().Vector();                         // 前
		const FVector R = FRotationMatrix(PlayerCharacter->GetActorRotation()).GetScaledAxis(EAxis::Y); // 右
		const FVector U = FRotationMatrix(PlayerCharacter->GetActorRotation()).GetScaledAxis(EAxis::Z); // 上
		const FVector Out = (-F + R + U).GetSafeNormal();
		
	
		//UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockEvaluator::OnRun: FocusLocation : %s"), *FocusLocation.ToString());

		bool bPitchAtLimit = DesiredRot.Pitch > MaxPitch ||
			DesiredRot.Pitch < MinPitch;

		if (bPitchAtLimit) {

			Margin *= BoomOffsetNode->PitchLimitMarginScale;
		}

		

		
		if (CaculateCameraRotate(PlayerLocation, TargetLocation, GetBoomArm(OutResult), DesiredRot))
		{
	
			LastCameraRot = UKismetMathLibrary::RInterpTo_Constant(LastCameraRot, DesiredRot, Params.DeltaTime, BoomOffsetNode->CameraRotMoveSpeed);
	
		
		}
		
		DesiredRot = LastCameraRot;
		DesiredRot.Pitch = FMath::Clamp(DesiredRot.Pitch, MinPitch, MaxPitch);
		UE_LOG(LogTemp, Error, TEXT("DesiredRot: %s"), *DesiredRot.ToString());


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
		
		/*auto CameraPos = [&](float Lenght)->FVector
			{
				return PlayerLocation + Out * Lenght;
			};*/

		auto BoomOffset = [&](float Lenght)->FVector
			{
				return FVector(-Lenght,CurrentArmValue.BoomLatera , CurrentArmValue.BoomHeight);
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


		TArray<FVector, TInlineAllocator<16>> TargetPts;

		TargetPts.Add(PlayerLocation + FVector(0, 0, BoomOffsetNode->PlayerHalfHeight));
		TargetPts.Add(PlayerLocation - FVector(0, 0, BoomOffsetNode->PlayerHalfHeight));
		TargetPts.Add(TargetLocation + FVector(0, 0, BoomOffsetNode->EnemyHalfHeight));
		TargetPts.Add(TargetLocation - FVector(0, 0, BoomOffsetNode->EnemyHalfHeight));

		const FVector CamLoc = OutResult.CameraPose.GetLocation();
		const FRotator CamRot = OutResult.CameraPose.GetRotation();
		const FTransform CamTM(CamRot, CamLoc);

		TargetPts.Add(PlayerLocation - CamTM.TransformVector(FVector(0, BoomOffsetNode->PlayerHalfWeight, 0)));
		TargetPts.Add(PlayerLocation + CamTM.TransformVector(FVector(0, BoomOffsetNode->PlayerHalfWeight, 0)));
		TargetPts.Add(TargetLocation + CamTM.TransformVector(FVector(0, BoomOffsetNode->EnemyHalfWeight, 0)));
		TargetPts.Add(TargetLocation - CamTM.TransformVector(FVector(0, BoomOffsetNode->EnemyHalfWeight, 0)));





		auto Fits = [&](float Length)->bool {

			FVector CamLoc = CameraPos(Length);

			auto Inside = [&](FVector Checkloc)->bool {
				const FVector Rel = Checkloc - CamLoc;
				const float Depth = FVector::DotProduct(Rel, ForwardAfterFrame(Length));
				if (Depth <= KINDA_SMALL_NUMBER)
				{
					return false;
				}
				const float r = FMath::Abs(FVector::DotProduct(Rel, RightAfterFrame(Length)));
				const float u = FMath::Abs(FVector::DotProduct(Rel, UpAfterFrame(Length)));

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





		float Hi = CurrentArmValue.MaxBoomLength;
		float Lo = CurrentArmValue.MinBoomLength;


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



		//获得当前Boom数值
		FVector CurrentBoom = GetBoomArm(OutResult);
		//UE_LOG(LogTemp, Warning, TEXT("CurrentBoom: %s"), *CurrentBoom.ToString());
		//获得计算数值

		const FVector NewCamWorld = PlayerLocation + Out * Hi;
		/*FVector FinalBoomOffset = FVector::ZeroVector;

		const TArrayView<const FCameraRigJoint> Joints = OutResult.CameraRigJoints.GetJoints();
		if (Joints.Num() > 0)
		{
			const FTransform& JointTM = Joints.Last().Transform;
			FinalBoomOffset = JointTM.InverseTransformPosition(NewCamWorld);
		}*/
	
	

		FVector FinalBoomOffset = FVector(-Hi, CurrentArmValue.BoomLatera, CurrentArmValue.BoomHeight);
		float BoomZoomSpeed = BoomZoomSpeedReader.Get(OutResult.VariableTable);
		FinalBoomOffset = UKismetMathLibrary::VInterpTo_Constant(CurrentBoom, FinalBoomOffset, Params.DeltaTime, BoomZoomSpeed);
	
		if (UVector3dCameraVariable* Var = BoomOffsetNode->BoomOffset.Variable)
		{
			OutResult.VariableTable.SetValue(Var, FinalBoomOffset);
		}


		FRotator CurrentCamerROT = OutResult.CameraPose.GetRotation();
	
		FVector C = PlayerLocation + DesiredRot.RotateVector(FinalBoomOffset);
		FRotator FinalRot = UKismetMathLibrary::FindLookAtRotation(C, FocusLocation);
		// 若本帧 CameraPose.Location 已经是摆好的相机，也可用 GetLocation()
		FinalRot.Pitch = FMath::Clamp(FinalRot.Pitch, MinPitch, MaxPitch);



		auto HorizBearingYaw = [](const FVector& From, const FVector& To) -> float
			{
				const FVector D = To - From;
				return FMath::RadiansToDegrees(FMath::Atan2(D.Y, D.X));
			};

		
		if (!bOrbitYawInitialized)
		{
			LastOrbitYaw = DesiredRot.Yaw;
			LastBoom = FinalBoomOffset;
			LastPlayerLoc = PlayerLocation;
			LastFocusLoc = FocusLocation;
			bOrbitYawInitialized = true;
		}

		const float BoomChange = (FinalBoomOffset - LastBoom).Size();
		const float PlayerMove = FVector::Dist(PlayerLocation, LastPlayerLoc);
		const float FocusMove = FVector::Dist(FocusLocation, LastFocusLoc);
		const float BearingDelta = FMath::Abs(FMath::FindDeltaAngleDegrees(
			HorizBearingYaw(LastPlayerLoc, LastFocusLoc),
			HorizBearingYaw(PlayerLocation, FocusLocation)));


		const bool bBoomChanging = BoomChange > 5.f;
		const bool bSceneMoving = PlayerMove > 1.f || FocusMove > 1.f || BearingDelta > 1.f;


		// 预测：若本帧 Orbit 跟到 DesiredLook，下一站还要不要同向再拧
		const float DeltaNow = FMath::FindDeltaAngleDegrees(LastOrbitYaw, DesiredRot.Yaw);

		const FVector C_ifFollow = PlayerLocation + DesiredRot.RotateVector(FinalBoomOffset);
		const FRotator Look_ifFollow = UKismetMathLibrary::FindLookAtRotation(C_ifFollow, FocusLocation);
		const float DeltaNext = FMath::FindDeltaAngleDegrees(DesiredRot.Yaw, Look_ifFollow.Yaw);


		const bool bNearFixed = FMath::IsNearlyZero(DeltaNext, 0.5f);
		const bool bCorrecting = (DeltaNow * DeltaNext) <= 0.f;
	/*	const bool bConverging = FMath::Abs(DeltaNext) + 0.5f < FMath::Abs(DeltaNow);*/
		const bool bSameDirChase =
			(DeltaNow * DeltaNext > 0.f) &&
			!FMath::IsNearlyZero(DeltaNow, 0.01f) &&
			!FMath::IsNearlyZero(DeltaNext, 0.01f);

		const float DistNow = FVector::Dist2D(PlayerLocation, FocusLocation);
		const float BoomR = FMath::Sqrt(FMath::Square(FinalBoomOffset.X) + FMath::Square(FinalBoomOffset.Y));
		const bool bNearSingularity = BoomR > DistNow * 0.85f;

		bool bDeadLoop =
			bSameDirChase &&
			bNearSingularity&&
			!bNearFixed &&
			!bCorrecting &&
			!bBoomChanging &&
			!bSceneMoving;



	/*	UE_LOG(LogTemp, Warning, TEXT("bSameDirChase: %i"), bSameDirChase);
		UE_LOG(LogTemp, Warning, TEXT("bNearFixed: %i"), bNearFixed);
		UE_LOG(LogTemp, Warning, TEXT("bCorrecting: %i"), bCorrecting);
		UE_LOG(LogTemp, Warning, TEXT("bBoomChanging: %i"), bBoomChanging);
		UE_LOG(LogTemp, Warning, TEXT("bSceneMoving: %i"), bSceneMoving);
		UE_LOG(LogTemp, Warning, TEXT("bDeadLoop: %i"), bDeadLoop);*/


		
		/*if(!bDeadLoop)*/
		OutResult.CameraPose.SetRotation(DesiredRot);


		LastBoom = FinalBoomOffset;
		LastPlayerLoc = PlayerLocation;
		LastFocusLoc = FocusLocation;
	
		UE_LOG(LogTemp, Warning, TEXT("PlayerLocation: %s"), *PlayerLocation.ToString());
		UE_LOG(LogTemp, Warning, TEXT("FocusLocation: %s"), *FocusLocation.ToString());
		UE_LOG(LogTemp, Warning, TEXT("TargetLocation: %s"), *TargetLocation.ToString());
		UE_LOG(LogTemp, Warning, TEXT("FinalBoomOffset: %s"), *CurrentBoom.ToString());
		
		UE_LOG(LogTemp, Warning, TEXT("DesiredRot: %s"), *DesiredRot.ToString());



	}

	float FCaBoomOffsetInLockEvaluator::GetTargetDistance(const FCameraNodeEvaluationParams& Params, FCameraNodeEvaluationResult& OutResult)
	{
		const UCaBoomOffsetInLock* BoomOffsetNode = GetCameraNodeAs<UCaBoomOffsetInLock>();
		if (!BoomOffsetNode)
		{
			UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockEvaluator::OnRun: BoomOffsetNode is null"));
			return 0.f;
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
			UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockEvaluator::OnRun: ContextOwner is not APlayerCharacter"));
			return 0.f;
		}

		AActor* TargetActor = PlayerCharacter->TargetActor;
		if (!TargetActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockEvaluator::OnRun: TargetActor is null"));
			return 0.f;
		}

		const float float_CenterToEnemy = CenterToEnemy.Get(OutResult.VariableTable);
		const FVector TargetLocation = TargetActor->GetActorLocation();
		const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
		return FVector::Distance(TargetLocation, PlayerLocation);
	}

	bool FCaBoomOffsetInLockEvaluator::CaculateCameraRotate(const FVector& PlayerLoc, const FVector& TargetLoc, const FVector& BoomArm, FRotator& OutRot)
	{
		const FVector D = TargetLoc - PlayerLoc;
		const float Bx = BoomArm.X;
		const float By = BoomArm.Y;
		const float Bz = BoomArm.Z;

		const float Rho = FMath::Sqrt(D.X * D.X + D.Y * D.Y);
		if (Rho <= KINDA_SMALL_NUMBER || FMath::Abs(By) > Rho + KINDA_SMALL_NUMBER)
		{
			return false; // 无精确解（侧向过大或目标几乎在头顶/脚下）
		}

		const float Beta = FMath::Atan2(D.Y, D.X); // 弧度
		const float AsinArg = FMath::Clamp(By / Rho, -1.f, 1.f);
		const float A = FMath::Asin(AsinArg);
		// 两个 Yaw 候选
		const float YawCandidates[2] = {
			Beta - A,
			Beta - (PI - A)
		};
		auto TryPitch = [&](float YawRad, float& OutPitchRad, float& OutLambda) -> bool
			{
				const float Cψ = FMath::Cos(YawRad);
				const float Sψ = FMath::Sin(YawRad);
				const float H = D.X * Cψ + D.Y * Sψ;
				const float L = FMath::Sqrt(H * H + D.Z * D.Z);
				if (L <= KINDA_SMALL_NUMBER || FMath::Abs(Bz) > L + KINDA_SMALL_NUMBER)
				{
					return false;
				}
				const float Phi = FMath::Atan2(H, D.Z);
				const float Ac = FMath::Acos(FMath::Clamp(Bz / L, -1.f, 1.f));
				const float PitchCandidates[2] = { Ac - Phi, -Ac - Phi };
				for (float PitchRad : PitchCandidates)
				{
					const float Cθ = FMath::Cos(PitchRad);
					const float Sθ = FMath::Sin(PitchRad);
					const float Alpha = H * Cθ + D.Z * Sθ;
					const float Lambda = Alpha - Bx;
					if (Lambda > KINDA_SMALL_NUMBER)
					{
						// 可选：验证 C 后 LookAt 是否一致
						OutPitchRad = PitchRad;
						OutLambda = Lambda;
						return true;
					}
				}
				return false;
			};
		for (float YawRad : YawCandidates)
		{
			float PitchRad = 0.f;
			float Lambda = 0.f;
			if (TryPitch(YawRad, PitchRad, Lambda))
			{
				OutRot = FRotator(
					FMath::RadiansToDegrees(PitchRad),
					FMath::RadiansToDegrees(YawRad),
					0.f);
				return true;
			}
		}
		return false;
	}



	

	

	void FCaBoomOffsetInLockEvaluator::OnInitialize(const FCameraNodeEvaluatorInitializeParams& Params, FCameraNodeEvaluationResult& OutResult)
	{
		const UCaBoomOffsetInLock* BoomOffsetNode = GetCameraNodeAs<UCaBoomOffsetInLock>();
		if (!BoomOffsetNode)
		{
			return;
		}
		ReframeDamper.Reset(0.f, 0.f);
		ReframeDamper.SetW0(10.f);

		LastOrbitYaw = 0.f;
		bOrbitYawInitialized = false;

		CurrentArmValue = BoomOffsetNode->InitalArmValue;
		// 参数若被 Variable / Interface 驱动，.Value 可能是 0；default 在 Variable 上，需用 Reader。
		MinPitchReader.Initialize(BoomOffsetNode->MinPitch);
		MaxPitchReader.Initialize(BoomOffsetNode->MaxPitch);
		CenterToEnemy.Initialize(BoomOffsetNode->CenterToEnemy);
		BoomZoomSpeedReader.Initialize(BoomOffsetNode->BoomZoomSpeed);
		BoomOffsetReader.Initialize(BoomOffsetNode->BoomOffset);
		FocusLocationReader.Initialize(BoomOffsetNode->FocusTarget);
		//进入时，焦点位置设置
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
		if (!PlayerCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockEvaluator::OnRun: ContextOwner is not APlayerCharacter"));
			return;
		}

		AActor* TargetActor = PlayerCharacter->TargetActor;
		if (!TargetActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("FCaBoomOffsetInLockEvaluator::OnRun: TargetActor is null"));
			return;
		}

		const float float_CenterToEnemy = CenterToEnemy.Get(OutResult.VariableTable);
		const FVector TargetLocation = TargetActor->GetActorLocation();
		const FVector PlayerLocation = PlayerCharacter->GetActorLocation();


		FVector IntLocation = UKismetMathLibrary::VLerp(PlayerLocation, TargetLocation, UKismetMathLibrary::FClamp(float_CenterToEnemy, 0, 1));

		if (UVector3dCameraVariable* Var = BoomOffsetNode->FocusTarget.Variable)
		{
			OutResult.VariableTable.SetValue(Var, IntLocation);
		}

		//

	
		CaculateCameraRotate(PlayerLocation, IntLocation, FVector(-BoomOffsetNode->InitalArmValue.MinBoomLength, BoomOffsetNode->InitalArmValue.BoomLatera, BoomOffsetNode->InitalArmValue.BoomHeight), LastCameraRot);



		
		UMC = OwnerActor->GetComponentByClass<UCamerManagerComponent>();
		LockModeChangedHandle = UMC->FOnCameraLockModeChangeDelegate.AddRaw(this, &FCaBoomOffsetInLockEvaluator::OnCameraLockModeChanged);
		BoomArmValueChangedHandle = UMC->FOnCameraBoomArmValueChangeDelegate.AddRaw(this, &FCaBoomOffsetInLockEvaluator::OnCameraArmValueChanged);
	
	}

	void FCaBoomOffsetInLockEvaluator::OnRun(const FCameraNodeEvaluationParams& Params, FCameraNodeEvaluationResult& OutResult)
	{
		CABoomArm(Params, OutResult);
	}

} // namespace UE::Cameras

FCameraNodeEvaluatorPtr UCaBoomOffsetInLock::OnBuildEvaluator(FCameraNodeEvaluatorBuilder& Builder) const
{
	using namespace UE::Cameras;
	return Builder.BuildEvaluator<FCaBoomOffsetInLockEvaluator>();
}

