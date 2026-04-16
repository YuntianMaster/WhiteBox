// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MyAnimInstance.h"

void UMyAnimInstance::UpdateVelocity()
{
	APawn* apawnref{ TryGetPawnOwner()};
	if (!IsValid(apawnref)) return;
	FVector velocity{ apawnref->GetVelocity() };
	Currentvelocity =static_cast<float>(velocity.Length());
	
}
 