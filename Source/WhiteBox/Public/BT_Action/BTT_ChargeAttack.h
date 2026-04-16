// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"
#include "BTT_ChargeAttack.generated.h"


struct FPathFollowingResult;
struct FAIRequestID;
/**
 * 
 */
UCLASS()
class WHITEBOX_API UBTT_ChargeAttack : public UBTTaskNode
{
	GENERATED_BODY()
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	FScriptDelegate FMoveCompleteDelegate;
public:

	UBTT_ChargeAttack();
	void ChargeAtPlayer();
	class AAIController* ControllerRef;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float AcceptanceRadius{ 150.0f };
	UFUNCTION()
	void HandleMoveCompleted();
	UFUNCTION()
	void FinishAttackTask();

	float WalkMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChargeMovementSpeed{ 2000.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChaseDuration{ 2.5f };
	class ABossCharater* CharacterRef;
	class UBlackboardComponent* BlackBoardComp;
	UFUNCTION()
	void HandleAttackFinished();
	UPROPERTY(BlueprintReadOnly)
	bool bIsFinished{};
	UFUNCTION(BlueprintCallable)
	void ChangeAttackHandle();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeleeRange{ 200.0f };
	bool bAttackIsFinish{false};
	



protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	bool bHasBoundMoveComplete{ false };

	
};
