// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/QTE_Base_Widget.h"
#include "Structure/FUIStruct.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/RadialSlider.h"
#include "Components/Image.h"
#include "components/CanvasPanelSlot.h"
#include "Components/Overlay.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"



void UQTE_Base_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!bIsQTEActive)
	{
		return;
	}

	SetVisibility(ESlateVisibility::Visible);
	if (QTE_Current_Mask)
	{
		// 输入改由 QTESystemComp 只轮询队首，避免同键同帧连锁触发
		QTE_Current_Mask->SetVisibility(
			bIsCurrentQTE ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}

	// Widget Tick 传入的 InDeltaTime 已被 GlobalTimeDilation 缩放。
	// QTE 反应时间按真实时间走，SlowRate 只影响世界，不影响倒计时。
	const UWorld* World = GetWorld();
	const float RealDelta = World ? World->DeltaRealTimeSeconds : InDeltaTime;
	QTE_RemaingTime += RealDelta;
	OnUpdateRemaingTime();

	if (QTE_RemaingTime >= QTE_Time)
	{
		HandleQTEResult();
	}
}

void UQTE_Base_Widget::CreateQTE(float QTETime, FName BoneName)
{

	if (!QTETable || !IMC_QTE||!IA_QTE) return;

	// 先标记本轮已激活，再停动画。
	// 否则上一轮 FailAnim 的 Finished（或 Stop 触发的 Finished）会把 Widget Collapsed，
	// Collapsed 后不再 Tick，逻辑 Timer 仍跑 → 有 Log 但不显示。
	bIsQTEActive = true;
	StopAllAnimations();
	SetVisibility(ESlateVisibility::Visible);

	if (QTE_Icon)
	{
		QTE_Icon->SetVisibility(ESlateVisibility::Visible);
		QTE_Icon->SetRenderScale(FVector2D(1.f, 1.f));
		QTE_Icon->SetColorAndOpacity(FLinearColor::White);
	}

	if (RemaingTimeSlider)
	{
		RemaingTimeSlider->SetVisibility(ESlateVisibility::Visible);
	}
	
	ACharacter* CharRef = Cast<ACharacter>(GetOwningPlayerPawn());
	if (CharRef)
	{
		FVector2D ScreenPos;
		FVector2D PelvisScreenPos;
		int32 SizeX = 0, SizeY = 0;
		FVector WorldLoc = CharRef->GetMesh()->GetSocketLocation(BoneName);
		FVector Pelvisloc = CharRef->GetMesh()->GetSocketLocation("pelvis");
		bool bHit = GetOwningPlayer()->ProjectWorldLocationToScreen(WorldLoc, ScreenPos);
		GetOwningPlayer()->ProjectWorldLocationToScreen(Pelvisloc, PelvisScreenPos);
		GetOwningPlayer()->GetViewportSize(SizeX, SizeY);
		float X_Direction = PelvisScreenPos.X - ScreenPos.X;
		float Y_Direction = PelvisScreenPos.Y - ScreenPos.Y;
		float X_Offset = LocOffset.X * FMath::Sign(X_Direction);
		float Y_Offset = LocOffset.Y * FMath::Sign(Y_Direction);
		FVector2D Normalized(ScreenPos.X / SizeX, ScreenPos.Y / SizeY);
		if (bHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("ScreenPos: %s"), *ScreenPos.ToString());
			UCanvasPanelSlot* OverlaySlot = Cast<UCanvasPanelSlot>(QTE_Overlay->Slot);
			OverlaySlot->SetAnchors(FAnchors(Normalized.X - X_Offset, Normalized.Y - Y_Offset, Normalized.X - X_Offset, Normalized.Y - Y_Offset));
			OverlaySlot->SetAlignment(FVector2D(0.5f, 0.5f));
			OverlaySlot->SetPosition(FVector2D(0.f, 0.f));
			OverlaySlot->SetAutoSize(false);
		}
	}
	SetVisibility(ESlateVisibility::Visible);
	
	QTE_Time = QTETime;
	TArray<FQTEPromptRow*> Rows;
	QTETable->GetAllRows(TEXT("QTE"), Rows);
	OnQTELoadReadyDeleagate.AddUniqueDynamic(this, &UQTE_Base_Widget::OnQTEPromptReady);

	UE_LOG(LogTemp, Warning, TEXT("QTE START!"));
	UE_LOG(LogTemp, Warning, TEXT("QTETime: %f!"), QTETime);
	if (Rows.Num() == 0) return;
	const FQTEPromptRow* Row = Rows[FMath::RandRange(0, Rows.Num() - 1)];
	ExpectedKey = Row->Key;

	if (UTexture2D* Tex = Row->Image.LoadSynchronous())
	{
		OnQTELoadReadyDeleagate.Broadcast(ExpectedKey,Tex);
	}

	UInputMappingContext* TempIMC = DuplicateObject(IMC_QTE, this);
	TempIMC->UnmapAll(); 
	TempIMC->MapKey(IA_QTE, ExpectedKey);


	ActiveQTEIMC = TempIMC;

	if (GetOwningPlayerPawn())
	{
		if (ULocalPlayer* LP = GetOwningPlayer()->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsys = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
				Subsys->AddMappingContext(ActiveQTEIMC, /*Priority*/ 100);
				Subsys->RequestRebuildControlMappings();

			}


			

			if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(GetOwningPlayerPawn()->InputComponent))
			{

				//EIC->BindAction(IA_QTE, ETriggerEvent::Completed, this, &UQTE_Base_Widget::HandleQTESuccess);
			}

		}
	}

	bIsSuccess = false;
	QTE_RemaingTime = 0.f;
	if (RemaingTimeSlider)
	{
		RemaingTimeSlider->SetValue(0.f);
	}
}

void UQTE_Base_Widget::HandleQTESuccess()
{
	bIsSuccess = true;
	HandleQTEResult();
}

void UQTE_Base_Widget::HandleQTEResult()
{
	bIsCurrentQTE = false;
	// 先结束本轮，避免广播里重入 CreateQTE 后，函数末尾再把新一轮掐掉
	if (!bIsQTEActive)
	{
		return;
	}
	bIsQTEActive = false;
	if (QTE_Current_Mask)
	{
		QTE_Current_Mask->SetVisibility(ESlateVisibility::Hidden);
	}

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsys = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsys->RemoveMappingContext(ActiveQTEIMC);
			}
		}
	}

	if (bIsSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("QTE SUCCESS!"));
		// 先 Broadcast 回收对象池，再 Brocast 给蓝图（蓝图里可立刻 CreateQTE）
		OnQTESuccess.Broadcast();
		OnQTESuccessBrocast();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("QTE Fail!"));
		OnQTEFail.Broadcast();
		OnQTEFailBrocast();
	}
}

void UQTE_Base_Widget::OnQTEPromptReady(FKey Key, UTexture2D* Image)
{
	UE_LOG(LogTemp, Warning, TEXT("QTE Change Image"));
	QTE_Icon->SetBrushFromTexture(Image);

}

void UQTE_Base_Widget::OnUpdateRemaingTime()
{
	if (RemaingTimeSlider && QTE_Time > 0.f)
	{
		RemaingTimeSlider->SetValue(FMath::Clamp(QTE_RemaingTime / QTE_Time, 0.f, 1.f));
	}
}

