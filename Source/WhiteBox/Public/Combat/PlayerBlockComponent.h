// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerBlockComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnBlockCostSignature,
	UPlayerBlockComponent,OnBlockCostDelegate,
	float,cost
);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHITEBOX_API UPlayerBlockComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerBlockComponent();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UAnimMontage* BlockAnim;
	UPROPERTY(BlueprintAssignable)
	FOnBlockCostSignature OnBlockCostDelegate;

	UFUNCTION()
	void RegenMovement();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	ACharacter* CharacterRef;
	class UCharacterMovementComponent* MoveComp;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION()
	bool Check(AActor* Opponent);

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float BlockingStaminaCost{10.0f};

	
		
};
