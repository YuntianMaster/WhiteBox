// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enum/EPlayerStates.h"
#include "FMovementAnimStruct.generated.h"
class UAnimSequenceBase;

/**
 * 内层方向 -> 动画映射。UHT 不支持 TMap 嵌套，必须用 USTRUCT 包一层。
 */
USTRUCT(BlueprintType)
struct WHITEBOX_API FMovementAnimStruct
{
	GENERATED_BODY()

	FMovementAnimStruct()
	{
		EnsureDefaultDirections();
	}

	/** 预填全部方向键，已有条目保留原值，缺失的补 nullptr（编辑器显示 None） */
	void EnsureDefaultDirections()
	{
		static constexpr EPlayerVelocityDirection AllDirections[] = {
			EPlayerVelocityDirection::VelocityNone,
			EPlayerVelocityDirection::VelocityForward,
			EPlayerVelocityDirection::VelocityBackward,
			EPlayerVelocityDirection::VelocityLeft,
			EPlayerVelocityDirection::VelocityRight,
			EPlayerVelocityDirection::VelocityFR,
			EPlayerVelocityDirection::VelocityFL,
			EPlayerVelocityDirection::VelocityBR,
			EPlayerVelocityDirection::VelocityBL,
		};

		for (EPlayerVelocityDirection Dir : AllDirections)
		{
			DirectionAnims.FindOrAdd(Dir);
		}
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TEnumAsByte<EPlayerVelocityDirection>, UAnimSequenceBase*> DirectionAnims;
};

/**
 * 中层 AnimState -> 方向映射。UHT 不支持 TMap 嵌套，必须用 USTRUCT 包一层。
 */
USTRUCT(BlueprintType)
struct WHITEBOX_API FAnimStateAnimStruct
{
	GENERATED_BODY()

	FAnimStateAnimStruct()
	{
		EnsureDefaultAnimStates();
	}

	/** 预填全部 AnimState 键，并确保其内层方向表完整 */
	void EnsureDefaultAnimStates()
	{
		static constexpr EAnimState AllAnimStates[] = {
			EAnimState::AnimStateNone,
			EAnimState::AnimStart,
			EAnimState::AnimCycle,
			EAnimState::AnimEnd,
		};

		for (EAnimState State : AllAnimStates)
		{
			StateAnims.FindOrAdd(State).EnsureDefaultDirections();
		}
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TEnumAsByte<EAnimState>, FMovementAnimStruct> StateAnims;
};
