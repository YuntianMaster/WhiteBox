// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Animation/AnimMontage.h"
#include "GameFrameWork/Character.h"
#include "CombatComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FBattleModeSignature,
	UCombatComponent, FBattleModeDelegate
);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FAttackPerformedSignature,
	UCombatComponent, OnAttackPerformDelegate,
	float,Amout

);



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WHITEBOX_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombatComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> AttackAnimations;
	UPROPERTY(VisibleAnywhere)
	int CombotCounter{ 0 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFirstCanAttack{ true };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bComboAttack{ true };
	bool bCanComboInput{ true };
	UPROPERTY(BlueprintAssignable)
	FAttackPerformedSignature OnAttackPerformDelegate;
	UPROPERTY(BlueprintAssignable)
	FBattleModeSignature FBattleModeDelegate;
	UFUNCTION(BlueprintCallable)
	void OnAttackPerformed();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaCost{ 5.0f };
	float AttackDurationTime{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> SkillAnimations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ChangetoTravel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChangetoTravelTime{ 2.f };
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	class APlayerCharacter* CharacterRef;
	class UPlayerAnimInstance* AnimInstance;
	UFUNCTION(BlueprintCallable)
	void UpdateResetAttack();

	void RandomAttack();

	UFUNCTION(BlueprintCallable)
	void ComboAttackHandle();
	UFUNCTION()
	void ChangeToTravelMode();
	UFUNCTION(BlueprintCallable)
	void BlockAttackHandle();
	UFUNCTION(BlueprintCallable)
	void CancelBlockAttackHandle();
	UFUNCTION(BlueprintCallable)
	void CombatThrow();

	void DefenseAttackHandle();
	void RollingAttackHandle();
	UFUNCTION(BlueprintCallable)
	void AttackFunction();
	class UUnlockActionComponent* LockActionRef;
	class UWeaponSystemComp* WeanponSystemRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackTimingPercent{ 0.3f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackBufferTime{ 0.3f };

	FTimerHandle FCombotInputHandle;
	FTimerHandle FCombotInputPassHandle;
	UFUNCTION()
	void ComboInputHandle();
	UFUNCTION()
	void ComboInputPassHandle();

	bool bIsAttackInput{ true };
	bool bIsBlockHandle{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WarpMotion")
	float MotionWarpDistance{ 300.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WarpMotion", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float TargetOffset{ 0.8f };
};