// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enum/InputDirection.h"
#include "Structure/FTakeDownAnimStruct.h"
#include "PlayerActionsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnSprintSignature,
	UPlayerActionsComponent, OnSprintDelegate,
	float, Amout


);

class UMotionWarpingComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHITEBOX_API UPlayerActionsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerActionsComponent();
	ACharacter* CharacterRef;
	class UCharacterMovementComponent* MovementComp;
	class IMainPlayerInterface* IplayerRef;

	UMotionWarpingComponent* MotionWarpingComp;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float SprintSpeed{ 1000.0f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WalkSpeed{300.0f};
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SprintStamniaCost{ 0.1f };
	UPROPERTY(BlueprintAssignable)
	FOnSprintSignature OnSprintDelegate;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator PlayerRotator{};
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator PlayerRotator_Input{};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* Walk_F45_Start;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void Sprint();
	UFUNCTION(BlueprintCallable)
	void GoWalk();
	UFUNCTION(BlueprintCallable)
	void Roll();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UAnimMontage* RollAnim;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RollStamniaCost{ 10.f };

	float bIsRollActive{ false };
	UFUNCTION()
	void FinishRollAnim();

	UFUNCTION(BlueprintCallable)
	void PlayerInteraceHandle();
	UPROPERTY(BlueprintReadWrite)
	bool bIsCrouching{ false };

	UFUNCTION(BlueprintCallable)
	void CrouchingStartHandle();
	UFUNCTION(BlueprintCallable)
	void CrouchingEndHandle();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ZoomSpeed{ 60.f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ZoomCameraLength{ 700.f };

	void CamZoonIn();
	void CamZoonOut();
	bool CheckingCrouch();
	FTimerHandle ZoomOutHandle;
	FTimerHandle ZoomInHandle;

	UFUNCTION(BlueprintCallable)
	void PlayStartMontage();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool DebugMode{ true };
	bool bIsCrouchTopBlock{ false };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<TEnumAsByte<PlayerDirectionInput>, UAnimMontage*> DirectionStartAnim;
	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<PlayerDirectionInput> Direction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TakeDownCheckRadiu{200.f};
	bool bIsTakeDownchecked;
	void TakeDownTargetCheck();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FTakeDownAnimStruct> TakeDownMontage;
	UFUNCTION(BlueprintCallable)
	AActor* TakeDownAction();

	AActor* TakeDownTarget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UWeaponSystemComp* WeaponSystemComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ACharacter> ShadowRef;
	int RollIndex = 0;
	bool bIsComboRoll = false;
	FTimerHandle RollComboTimeHandler;
	FTimerHandle RollFinishTimeHandler;
	
	// Roll结束后，从滚动朝向平滑过渡到控制器朝向，避免一帧跳转
	UFUNCTION()
	void UpdatePostRollTurning();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Roll")
	float Speed = 100.f;
	FTimerHandle RollToController;

	UFUNCTION()
	void ComboRollAnim();

	UFUNCTION()
	void EndRollingHandle();
	
};
