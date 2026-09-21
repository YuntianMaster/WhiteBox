// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QTE_Base_Widget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(
	FOnQTELoadReadySingnature,
	UQTE_Base_Widget, OnQTELoadReadyDeleagate,
	FKey, Key,
	UTexture2D*, Image
);

DECLARE_MULTICAST_DELEGATE(FOnQTEFinishedSignature);


UCLASS()
class WHITEBOX_API UQTE_Base_Widget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UImage* QTE_Icon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UImage* QTE_Current_Mask;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class URadialSlider* RemaingTimeSlider;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UOverlay* QTE_Overlay;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "TRUE"))
	float QTE_Time{1.f};
	float QTE_RemaingTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QTE Settings")
	TObjectPtr<UDataTable> QTETable;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QTE Settings")
	class UInputMappingContext* IMC_QTE;
	UPROPERTY(EditAnywhere, Category = "QTE Settings")
	class UInputAction* IA_QTE;
	UPROPERTY(EditAnywhere, Category = "QTE Settings")
	FVector2D LocOffset;
	class UInputMappingContext* ActiveQTEIMC;
	FTimerHandle QTE_TimerHandle;
	FTimerHandle QTE_RemaingTimeHandle;
	bool bIsSuccess;
	UPROPERTY(BlueprintAssignable)
	FOnQTELoadReadySingnature OnQTELoadReadyDeleagate;

	UPROPERTY(BlueprintReadOnly, Category = "QTE")
	bool bIsQTEActive{ false };
	UPROPERTY(BlueprintReadOnly, Category = "QTE")
	bool bIsCurrentQTE{ false };
	FKey ExpectedKey;

public:
	UFUNCTION(BlueprintCallable)
	void CreateQTE(float QTETime, FName BoneName);
	
	void HandleQTESuccess();
	UFUNCTION()
	void HandleQTEResult();
	UFUNCTION()
	void OnQTEPromptReady(FKey Key, UTexture2D* Image);
	UFUNCTION()
	void OnUpdateRemaingTime();

	UFUNCTION(BlueprintImplementableEvent)
	void OnQTESuccessBrocast();
	UFUNCTION(BlueprintImplementableEvent)
	void OnQTEFailBrocast();


	FOnQTEFinishedSignature OnQTESuccess;
	FOnQTEFinishedSignature OnQTEFail;


	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	
};
