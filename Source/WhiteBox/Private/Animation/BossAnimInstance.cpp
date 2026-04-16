// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/BossAnimInstance.h"

void UBossAnimInstance::UpdateCurrentVerticleSpeed()
{
	APawn* PawnRef{ TryGetPawnOwner() };
	if (!IsValid(PawnRef)) return;
	FVector speed = PawnRef->GetVelocity();
	CurrentVerticleSpeed = speed.Length();
}
