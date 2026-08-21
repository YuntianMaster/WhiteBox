// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CamerManagerComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Core/CameraVariableTable.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"


// Sets default values for this component's properties
UCamerManagerComponent::UCamerManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCamerManagerComponent::SetInitialCameraPose()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	UGameplayCameraComponent* CGC = Owner->FindComponentByClass<UGameplayCameraComponent>();
	if (!CGC)
	{
		return;
	}


	APlayerController* PC = Cast<APlayerController>(Owner->GetInstigatorController());
	if (!PC)
	{
		PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	}
	if (!PC || !PC->PlayerCameraManager)
	{
		return;
	}

	// 仍在 Lock Rig 上时读取当前镜头旋转，供 Third Person Entry 的 Context Yaw Pitch 使用。
	const FRotator CameraRotation = PC->PlayerCameraManager->GetCameraRotation();

	FBlueprintCameraPose InitialPose = CGC->GetInitialPose();
	InitialPose.Rotation = CameraRotation;
	CGC->SetInitialPose(InitialPose);

	UE_LOG(LogTemp, Warning, TEXT("SetInitialCameraPose: Yaw=%f Pitch=%f"), CameraRotation.Yaw, CameraRotation.Pitch);

	Table = CGC->GetInitialVariableTable();
}


void UCamerManagerComponent::CameraChangeHandle(UPARAM(meta = (Categories = "Camera")) FGameplayTag CameraTag)
{
	if(CurrentCameraTag == CameraTag )
		return;

	CurrentCameraTag = CameraTag;
	FOnCameraChangeDelegate.Broadcast(CameraTag);
			
}


void UCamerManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

//void UCamerManagerComponent::CaculateBoomArmForLockingRig()
//{
//
//	APlayerCharacter* PlayerActor = Cast<APlayerCharacter>(GetOwner());
//	if (!PlayerActor)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("No PlayerActor"));
//		return;
//	}
//
//	AActor* EnemyActor = PlayerActor->TargetActor;
//	if (!EnemyActor)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("No EnemyActor"));
//		return;
//	}
//
//	PlayerLoc = PlayerActor->GetActorLocation();
//	EnemyLoc = EnemyActor->GetActorLocation();
//	APlayerCameraManager* CurrentCamera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
//	if (!CurrentCamera)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("No CurrentCamera"));
//		return;
//	}
//
//	VerticalFOVDeg = CurrentCamera->GetFOVAngle();
//	UE_LOG(LogTemp, Warning, TEXT("VerticalFOVDeg: %f"), VerticalFOVDeg);
//	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
//	{
//		if (const ULocalPlayer* LP = PC->GetLocalPlayer())
//		{
//			if (LP->ViewportClient)
//			{
//				FVector2D Size;
//				LP->ViewportClient->GetViewportSize(Size);
//				if (Size.Y > 0.f)
//				{
//					Aspect = Size.X / Size.Y;
//				}
//			}
//		}
//	}
//
//	// Table 仅在 SetInitialCameraPose 时缓存，锁定期间常为空。
//	// 夹角是 Collection 上的配置默认值，优先从变量资产读取；若有运行时表再覆盖。
//	float MinPitch = 0.f;
//	float MaxPitch = 0.f;
//
//	if (MinPitchVariable.IsValid())
//	{
//		MinPitch = MinPitchVariable.Get()->GetDefaultValue();
//	}
//	if (MaxPitchVariable.IsValid())
//	{
//		MaxPitch = MaxPitchVariable.Get()->GetDefaultValue();
//	}
//	UGameplayCameraComponent* CGC = PlayerActor->FindComponentByClass<UGameplayCameraComponent>();
//	if (CGC)
//	{
//		const FBlueprintCameraVariableTable LiveTable = CGC->GetInitialVariableTable();
//		if (LiveTable.IsValid())
//		{
//			Table = LiveTable;
//			if (MinPitchVariable.IsValid())
//			{
//				MinPitch = LiveTable.GetVariableTable()->GetValue(MinPitchVariable.Get());
//			}
//			if (MaxPitchVariable.IsValid())
//			{
//				MaxPitch = LiveTable.GetVariableTable()->GetValue(MaxPitchVariable.Get());
//			}
//		}
//	}
//
//	if (!MinPitchVariable.IsValid() || !MaxPitchVariable.IsValid())
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Pitch variables not assigned on CamerManagerComponent"));
//	}
//
//
//
//	//FRotator BoomRot = CurrentCamera->GetCameraRotation();
//	//UE_LOG(LogTemp, Warning, TEXT("BoomRot: %s"), *BoomRot.ToString());
//
//	//UE_LOG(LogTemp, Warning, TEXT("MinPitch: %f"), MinPitch);
//	//UE_LOG(LogTemp, Warning, TEXT("MaxPitch: %f"), MaxPitch);
//
//
//
//	const FVector Focus = FMath::Lerp(PlayerLoc, EnemyLoc, FMath::Clamp(CenterWeightToEnemy, 0.f, 1.f));
//	FRotator DesiredRot = UKismetMathLibrary::FindLookAtRotation(PlayerLoc, Focus);
//	DesiredRot.Pitch = FMath::Clamp(DesiredRot.Pitch, MinPitch, MaxPitch);
//
//
//
//	const FVector Forward = DesiredRot.Vector();
//	const FVector Right = FRotationMatrix(DesiredRot).GetScaledAxis(EAxis::Y);
//	const FVector Up = FRotationMatrix(DesiredRot).GetScaledAxis(EAxis::Z);
//
//	const float HalfV = FMath::DegreesToRadians(VerticalFOVDeg / 2);
//	const float HalfH = FMath::Atan(FMath::Tan(HalfV) * Aspect);
//	const float TanH = FMath::Tan(HalfH);
//	const float TanV = FMath::Tan(HalfV);
//
//
//	const float SafeH = TanH * ScreenMargin;
//	const float SafeV = TanV * ScreenMargin;
//
//	auto MakeBoomOffset = [&](float Length)->FVector {
//		return FVector(-Length, BoomLateral, BoomHeight);
//		};
//
//	auto CameraFormOffset = [&](FVector BoomOffset)->FVector {
//		return PlayerLoc + DesiredRot.RotateVector(BoomOffset);
//		};
//
//
//	auto  ForwarAfterFrame = [&](float Length)-> FVector {
//		const FVector CameraLoc = CameraFormOffset(MakeBoomOffset(Length));
//		const FRotator ROT = UKismetMathLibrary::FindLookAtRotation(CameraLoc, Focus);
//		return ROT.Vector();
//		};
//
//	auto RightAfterFrame = [&](float Length)-> FVector {
//		const FVector CameraLoc = CameraFormOffset(MakeBoomOffset(Length));
//		const FRotator ROT = UKismetMathLibrary::FindLookAtRotation(CameraLoc, Focus);
//		return FRotationMatrix(ROT).GetScaledAxis(EAxis::Y);
//		};
//
//	auto UpAfterFrame = [&](float Length)-> FVector {
//		const FVector CameraLoc = CameraFormOffset(MakeBoomOffset(Length));
//		const FRotator ROT = UKismetMathLibrary::FindLookAtRotation(CameraLoc, Focus);
//		return FRotationMatrix(ROT).GetScaledAxis(EAxis::Z);
//		};
//
//	TArray<FVector, TInlineAllocator<8>> Pts;
//	Pts.Add(PlayerLoc + FVector(0, 0, PlayerHalfHeight));  // 头
//	Pts.Add(PlayerLoc - FVector(0, 0, PlayerHalfHeight));  // 脚
//	Pts.Add(EnemyLoc + FVector(0, 0, EnemyHalfHeight));
//	Pts.Add(EnemyLoc - FVector(0, 0, EnemyHalfHeight));
//
//
//	auto Fit = [&](float Length) ->bool {
//		const FVector BoomOffset = MakeBoomOffset(Length);
//		const FVector CamLoc = CameraFormOffset(BoomOffset);
//			auto Inside = [&](const FVector WorldPoint) ->bool {
//				const FVector Rel = WorldPoint - CamLoc;
//				const float Depth = FVector::DotProduct(Rel, ForwarAfterFrame(Length));
//				if (Depth <= KINDA_SMALL_NUMBER)
//				{
//					return false;
//				}
//				const float r = FMath::Abs(FVector::DotProduct(Rel, RightAfterFrame(Length)));
//				const float u = FMath::Abs(FVector::DotProduct(Rel, UpAfterFrame(Length)));
//				return r <= Depth * SafeH && u <= Depth * SafeV;
//				};
//
//			for(const FVector& Pt : Pts)
//			{
//				if(!Inside(Pt))
//				{
//					return false;
//				}
//			}
//		return true;
//	};
//	
//
//	/*if(!Fit(MinBoomLength))
//	{
//		UE_LOG(LogTemp, Warning, TEXT("MinBoomLength too short to fit both actors"));
//		return;
//	}
//
//	if(!Fit(MaxBoomLength))
//	{
//		UE_LOG(LogTemp, Warning, TEXT("MaxBoomLength too long to fit both actors"));
//		return;
//	}*/
//	float Lo = MinBoomLength;
//	float Hi = MaxBoomLength;
//
//	for (int32 i = 0; i < 20; ++i)
//	{
//		const float Mid = 0.5f * (Lo + Hi);
//		if (Fit(Mid))
//		{
//			Hi = Mid; // 还能更近
//		}
//		else
//		{
//			Lo = Mid; // 要更远
//		}
//	}
//
//	Table.GetVariableTable()->SetValue(BoomLengthVariable.Get(), Hi);
//	UE_LOG(LogTemp, Warning, TEXT("MaxBoomLength: %f"), Hi);
//
//}





		//Side摄像机，需要两者侧面

		/*const FVector Mid = FMath::Lerp(PlayerLoc,EnemyLoc, FMath::Clamp(CenterWeightToEnemy, 0.f, 1.f));
		const float SpanH = FVector::Dist2D(PlayerLoc, EnemyLoc) + PlayerRadius + EnemyRadius;
		const float SpanV = FMath::Abs(PlayerLoc.Z - EnemyLoc.Z) + PlayerRadius + EnemyRadius;

		UE_LOG(LogTemp, Warning, TEXT("Mid: %s"), *Mid.ToString());

		const float HalfV = FMath::DegreesToRadians(VerticalFOVDeg / 2);
		const float HalfH = FMath::Atan(FMath::Tan(HalfV) * Aspect);

		const float InvSize = 1.f / FramingSize;

		const float DistH = SpanH *0.5f * InvSize / FMath::Max(FMath::Tan(HalfH),KINDA_SMALL_NUMBER);
		const float DistV = SpanV *0.5f * InvSize / FMath::Max(FMath::Tan(HalfV), KINDA_SMALL_NUMBER);

		UE_LOG(LogTemp, Warning, TEXT("DistH: %f"), DistH);
		UE_LOG(LogTemp, Warning, TEXT("DistV: %f"), DistV);
		const float FramingDistToMid = FMath::Max(DistH, DistV);

		const FVector BoomForward = BoomRot.Vector();
		const FVector IdealCam = Mid - FramingDistToMid * BoomForward;

		const FVector FromPivot = IdealCam - PlayerLoc;

		UE_LOG(LogTemp, Warning, TEXT("Cam: %s"), *IdealCam.ToString());

		float Boomlength = FVector::DotProduct(FromPivot, -BoomForward);

		UE_LOG(LogTemp, Warning, TEXT("Boom Length: %f"), Boomlength);*/

		


