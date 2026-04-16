// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structure/FTraceSockets.h"
#include "PlayerTraceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHITEBOX_API UPlayerTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerTraceComponent();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	class UParticleSystem* bloodParticle;
	class ACharacter* OwnRef;
	class UWeaponSystemComp* WeaponSystemComp;




protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BoxCollisionLenght;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bDebugMode{ false };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Trace|Optimize")
	bool bEnableSweptTrace{ true };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Trace|Optimize", meta = (ClampMin = "1", ClampMax = "12"))
	int32 SweptTraceMaxSubsteps{ 6 };

	// 运动距离超过该阈值会增加子步数（单位：cm）
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Trace|Optimize", meta = (ClampMin = "1.0"))
	float SweptTraceStepDistance{ 12.0f };

	struct FPrevSocketPair{
			FVector Start = FVector::ZeroVector;
			FVector End = FVector::ZeroVector;
			bool isVaild = false;
	};
	TMap<uint32, FPrevSocketPair> PrevSocketPairs;
	TArray<AActor*> TargetsToIngore;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool bIsAttacking{false};
	UFUNCTION(BlueprintCallable)
	void HandleResestAttack();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FTraceSockets> TraceSockets;



		
};
