// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/AnimInstanceLayersBase.h"
#include "Animation/EnemyAnimInstance.h"

void UAnimInstanceLayersBase::PostInitProperties()
{
	Super::PostInitProperties();

	for (auto& Pair : MovementAnimCollection)
	{
		Pair.Value.EnsureDefaultAnimStates();
	}
}

UAnimSequenceBase* UAnimInstanceLayersBase::GetMovementAnim(
	TEnumAsByte<E_Gate> Gate,
	TEnumAsByte<EAnimState> AnimState,
	TEnumAsByte<EPlayerVelocityDirection> Direction) const
{
	return MovementAnimCollection.FindRef(Gate).StateAnims.FindRef(AnimState).DirectionAnims.FindRef(Direction);
}

UEnemyAnimInstance* UAnimInstanceLayersBase::GetEnemyAnimInstance() const
{
	return Cast<UEnemyAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
