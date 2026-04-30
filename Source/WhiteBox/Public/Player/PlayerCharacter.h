// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/MainPlayerInterface.h"
#include "Animation/PlayerAnimInstance.h"
#include "Interface/Fighter.h"
#include "Player/StatsComponent.h"
#include "Combat/CombatComponent.h"
#include "Combat/LockComponent.h"
#include "Combat/PlayerTraceComponent.h"
#include "Combat/PlayerBlockComponent.h"
#include "Player/TravelingComp.h"
#include "Player/PlayerActionsComponent.h"
#include "Steal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Enum/EPlayerStates.h"
#include "PlayerCharacter.generated.h" 


UCLASS()
class WHITEBOX_API APlayerCharacter : public ACharacter,public IMainPlayerInterface,public IFighter,public ISteal, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	APlayerCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	UAbilitySystemComponent* AbilitySystemComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	class UBasicAttributeSet* AttributeSet;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	class UCombatAttributeSet* CombatAttributeSet;

	EGameplayEffectReplicationMode AscReplicationMode = EGameplayEffectReplicationMode::Mixed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	TArray<TSubclassOf<UGameplayAbility>> InitalAbilities;

	UFUNCTION()
	void OnDeadTagChange(const FGameplayTag CallBackTag, int32 Newcount);
	void OnBockingTagChange(const FGameplayTag Callbacktage, int32 NewCount);
	void OnAimingTagChange(const FGameplayTag Callbacktage, int32 NewCount);
	void OnDrawingTagChange(const FGameplayTag Callbacktage, int32 NewCount);
	
	UPROPERTY(BlueprintReadOnly)
	class UPlayerAnimInstance* PlayerAnim;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	class UStatsComponent* StatsComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UCombatComponent* CombatComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class ULockComponent* LockComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UPlayerTraceComponent* TraceComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UPlayerActionsComponent* ActionComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UPlayerBlockComponent* BlockComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTravelingComp* TravelingComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UCollectionComp* CollectionComp;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UClimbComponent* UClimbComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UUnlockActionComponent* LockActionComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UWeaponSystemComp* WeaponSystemComp;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UCamerManagerComponent* CameraManager;
	UFUNCTION()
	virtual float OnHandleDeath() override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EPlayerStates> PlayerStats{ EPlayerStates::CharacterNoneStats };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UPlayerMainWidget* PlayerHUD;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTalentComp* TalentComp;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float GetDamage() override;

	virtual bool HasEnoughStamina(float StaminaCost) override;

	void UnlockEnemy();

	virtual void EndLockonWithActor(AActor* ActorRef) override;

	virtual bool CanTakeDamage(AActor* Opponent) override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UAnimMontage* HurtAnimation;
	UFUNCTION(BlueprintCallable)
	void PlayHurtAnimation(TSubclassOf<UCameraShakeBase> CameraShakeTemp);

	virtual void EnablePlayerInput_Implementation() override;
	virtual void DisablePlayerInput_Implementation()override;

	UFUNCTION(BlueprintCallable)
	virtual void beOnDectedHandle() override;
	FTimerHandle FbeOnDectedHandle;
	UFUNCTION(BlueprintCallable)
	virtual void beOnUndectedHandle() override;
	FTimerHandle FbeOnUndectedHandle;


	void PlayerInteract();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool isInbattle{false};
};
