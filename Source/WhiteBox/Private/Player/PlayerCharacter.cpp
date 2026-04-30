// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"
#include "Player/StatsComponent.h"
#include "Combat/CombatComponent.h"
#include "Combat/LockComponent.h"
#include "Combat/PlayerTraceComponent.h"
#include "Combat/PlayerBlockComponent.h"
#include "BrainComponent.h"
#include "Enemy/BossCharater.h"
#include "Player/TravelingComp.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/PlayerActionsComponent.h"
#include "Player/ClimbComponent.h"
#include "Combat/WeaponSystemComp.h"
#include "Travel/CollectionComp.h"
#include "AbilitySystemComponent.h"
#include "AttrubuteSet/BasicAttributeSet.h"
#include "AttrubuteSet/CombatAttributeSet.h"
#include "Player/UnlockActionComponent.h"
#include "Player/CamerManagerComponent.h"
#include "Player/TalentComp.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("Stat Component"));
	StatsComp->CreationMethod = EComponentCreationMethod::Native;
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
	LockComp = CreateDefaultSubobject<ULockComponent>(TEXT("Lock Component"));
	TraceComp = CreateDefaultSubobject<UPlayerTraceComponent>(TEXT("Trace Component"));
	ActionComp = CreateDefaultSubobject<UPlayerActionsComponent>(TEXT("Action Component"));
	BlockComp = CreateDefaultSubobject<UPlayerBlockComponent>(TEXT("Block Component"));
	TravelingComp = CreateDefaultSubobject<UTravelingComp>(TEXT("Traveling Componet"));
	UClimbComp = CreateDefaultSubobject<UClimbComponent>(TEXT("Climb Component"));
	LockActionComp = CreateDefaultSubobject<UUnlockActionComponent>(TEXT("LockActionComp Component"));
	CollectionComp = CreateDefaultSubobject<UCollectionComp>(TEXT("CollectionComp"));
	WeaponSystemComp = CreateDefaultSubobject<UWeaponSystemComp>(TEXT("WeaponSystem"));
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UBasicAttributeSet>(TEXT("AttributeSet"));
	CameraManager = CreateDefaultSubobject<UCamerManagerComponent>(TEXT("CameraManager"));
	CombatAttributeSet = CreateOptionalDefaultSubobject<UCombatAttributeSet>(TEXT("CombatAttributeSet"));
	TalentComp = CreateDefaultSubobject<UTalentComp>(TEXT("TalentComp"));
	
	AbilitySystemComp->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("Stats.Dead"))
		.AddUObject(this,&APlayerCharacter::OnDeadTagChange);
	AbilitySystemComp->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("Stats.Blocking"))
		.AddUObject(this, &APlayerCharacter::OnBockingTagChange);
	AbilitySystemComp->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("Stats.Aiming"))
		.AddUObject(this, &APlayerCharacter::OnAimingTagChange);
	AbilitySystemComp->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("Stats.Drawing"))
		.AddUObject(this, &APlayerCharacter::OnDrawingTagChange);
}

UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void APlayerCharacter::OnDeadTagChange(const FGameplayTag CallBackTag, int32 Newcount)
{
	if(Newcount>0)
		UE_LOG(LogTemp, Warning, TEXT("Dead"));
}

void APlayerCharacter::OnBockingTagChange(const FGameplayTag Callbacktage, int32 NewCount)
{
	if (NewCount > 0)
		PlayerStats = EPlayerStates::SwordDFence;
	if (NewCount <= 0)
		PlayerStats = EPlayerStates::CharacterNoneStats;
}

void APlayerCharacter::OnAimingTagChange(const FGameplayTag Callbacktage, int32 NewCount)
{
	if (NewCount > 0)
	{
		PlayerStats = EPlayerStates::Aiming;
		isInbattle = true;
	}
	if (NewCount <= 0)
	{
		PlayerStats = EPlayerStates::CharacterNoneStats;
		isInbattle = false;
	}
}

void APlayerCharacter::OnDrawingTagChange(const FGameplayTag Callbacktage, int32 NewCount)
{
	if (NewCount > 0)
	{
		PlayerStats = EPlayerStates::Drawing;
		isInbattle = true;
	}
	if (NewCount <= 0)
	{
		PlayerStats = EPlayerStates::Aiming;
		isInbattle = false;
	}
}



float APlayerCharacter::OnHandleDeath()
{
	if (!IsValid(DeathAnimMontage)) return 0.f;
	APlayerController* PC = GetController<APlayerController>();
	if (PC)
	{
		DisableInput(PC);
	}
	
	return PlayAnimMontage(DeathAnimMontage);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerAnim =Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	AbilitySystemComp->SetIsReplicated(true);
	AbilitySystemComp->SetReplicationMode(AscReplicationMode);
	AbilitySystemComp->InitAbilityActorInfo(this, this);

	for (TSubclassOf<UGameplayAbility> ablility : InitalAbilities) {

		FGameplayAbilitySpecHandle SpecHandle  = AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(
			ablility,
			1,
			-1,
			this
		));

	}

}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if(AbilitySystemComp)
			AbilitySystemComp->InitAbilityActorInfo(this, this);
}

void APlayerCharacter::OnRep_PlayerState()
{

	Super::OnRep_PlayerState();
	if (AbilitySystemComp)
		AbilitySystemComp->InitAbilityActorInfo(this, this);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float APlayerCharacter::GetDamage()
{
	return StatsComp->Stats[EStats::Strength];
}

bool APlayerCharacter::HasEnoughStamina(float StaminaCost)
{
	if (StaminaCost > StatsComp->Stats[EStats::Stamina])
		return false; 
	else
		return true;
}

void APlayerCharacter::UnlockEnemy()
{
	LockComp->UnlockTarget();
}

void APlayerCharacter::EndLockonWithActor(AActor* ActorRef)
{

	if (ActorRef != LockComp->CurrentTargetActor) return;
	LockComp->UnlockTarget();

}

bool APlayerCharacter::CanTakeDamage(AActor* Opponent)
{


	if (ActionComp->bIsRollActive) return false;
	
	if (PlayerAnim->bIsBlocking){
		return BlockComp->Check(Opponent);
	}
	return true;
}

void APlayerCharacter::PlayHurtAnimation(TSubclassOf<UCameraShakeBase> CameraShakeTemp)
{
	PlayAnimMontage(HurtAnimation);

	if (CameraShakeTemp) {
		GetController<APlayerController>()->ClientStartCameraShake(CameraShakeTemp);
	}
}

void APlayerCharacter::EnablePlayerInput_Implementation()
{
	GetComponentByClass<UCharacterMovementComponent>()->SetMovementMode(MOVE_Walking);
}

void APlayerCharacter::DisablePlayerInput_Implementation()
{
	GetComponentByClass<UCharacterMovementComponent>()->SetMovementMode(MOVE_None);
}



void APlayerCharacter::beOnDectedHandle()
{
	GetWorldTimerManager().ClearTimer(FbeOnUndectedHandle);
	GetWorldTimerManager().SetTimer(
		FbeOnDectedHandle,
		StatsComp,
		&UStatsComponent::UpdateOnDetected,
		GetWorld()->DeltaTimeSeconds,
		true
	);
}

void APlayerCharacter::beOnUndectedHandle()
{
	GetWorldTimerManager().ClearTimer(FbeOnDectedHandle);
	GetWorldTimerManager().SetTimer(
		FbeOnUndectedHandle,
		StatsComp,
		&UStatsComponent::UpdateOnUndetected,
		GetWorld()->DeltaTimeSeconds,
		true,
		2.0f
	);
}






