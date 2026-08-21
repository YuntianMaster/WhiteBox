// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTNode.h"
#include "BT_Action/BTT_Base.h"
#include "Enum/ECombatEnum.h"
#include "BTT_SetMovementSpeed.generated.h"

/**
 * 
 */
UCLASS()
class WHITEBOX_API UBTT_SetMovementSpeed : public UBTT_Base
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CombatSpeed")
	bool bIsCutstomCombatSpeed{ false };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CombatSpeed", meta = (EditCondition = "bIsCutstomCombatSpeed"))
	float CustomCombatSpeed{ 2000.f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CombatSpeed", meta = (EditCondition = "!bIsCutstomCombatSpeed"))
	TEnumAsByte<ECombatSpeeds> CombatSpeeds{ ECombatSpeeds::WalkSpeed };

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
	
};
