// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Widget/DamagePropsWidget.h"
#include "Damageable.generated.h"


UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};
/**
 * 
 */

class WHITEBOX_API IDamageable
{
	GENERATED_BODY()
public:
	virtual void TakeDamage(AActor* Causer,float Damage){};
	virtual void UpdateHP(float Amount){}
	float MaxHP{};
	float CurrentHP{};

	

};
