// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ClimbComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHITEBOX_API UClimbComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UClimbComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void checkblocker();

	bool bDistanceCheck{ false };

	void PlayClimbMontage(int highIndex, FVector Loc);
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UAnimMontage* LowClimb;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* HighClimb;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* JumpClimb;
	FOnMontageEnded ClimbEnd;
	UFUNCTION()
	void MontageEndHandle();
	int HighLevel{0};

	class UUnlockActionComponent* LockComp;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bDebugMode{ true };
};
