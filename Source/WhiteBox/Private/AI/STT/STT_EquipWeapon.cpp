// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STT/STT_EquipWeapon.h"
#include "Enemy/EnemyCharacter.h"
#include "Combat/WeaponSystemComp.h"

EStateTreeRunStatus USTT_EquipWeapon::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	UWeaponSystemComp* WeaponSys = EnemyActor->WeaponSystemComp;
	EWeapons CurrentWeapon = WeaponSys->ECurrentWeapons;
	FTimerHandle TimerHandle;

	if (CurrentWeapon == WeaponName)
	{
		return EStateTreeRunStatus::Succeeded;
	}

	if (CurrentWeapon != WeaponName) {
		WeaponSys->SwitchWeapon(WeaponName);
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			[this]() {


				UE_LOG(LogTemp, Warning, TEXT("STT_Equip Weapon Complete!"));
				FinishTask(true);
			},
			WaitEquipTime,
			false
		);
	}



	return EStateTreeRunStatus::Running;
}
