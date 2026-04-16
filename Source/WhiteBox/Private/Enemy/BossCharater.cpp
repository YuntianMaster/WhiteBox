// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossCharater.h"
#include "Debug/DebugLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/Estats.h"
#include "Player/PlayerCharacter.h"
#include "Combat/CombatComponent.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/LookAtPlayerComp.h"
#include "Interface/MainPlayerInterface.h"
#include "Player/StatsComponent.h"

// Sets default values
ABossCharater::ABossCharater()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	statsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("Stat comp"));
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Comp"));

}

// Called when the game starts or when spawned
void ABossCharater::BeginPlay()
{
	Super::BeginPlay();
	BlackboardComp = GetController<AAIController>()->GetBlackboardComponent();
	BlackboardComp->SetValueAsEnum(
		TEXT("BossStats"),
		InitalStats
	);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(PC->GetPawn());

	if (!PlayerChar)
	{
		UE_LOG(LogTemp, Error, TEXT("Boss: PlayerChar is NULL"));
		return;
	}

	if (!PlayerChar->StatsComp)
	{
		UE_LOG(LogTemp, Error, TEXT("Boss: Player StatsComp is NULL"));
		return;
	}

	PlayerChar->StatsComp->OnHealthZeroDelegate.AddDynamic(  
		this, &ABossCharater::HandleGameOver);

	UE_LOG(LogTemp, Warning, TEXT("Boss: HandleGameOver BOUND SUCCESSFULLY"));
	
}

float ABossCharater::OnHandleDeath()
{
	float DeathDuraton = PlayAnimMontage(DeathAnim) + 1.0f;
	GetController<AAIController>()->GetBrainComponent()->StopLogic("Defeated");
	FindComponentByClass<UCapsuleComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetComponentByClass<ULookAtPlayerComp>()->bCanLookAt = false;
	OnEnemyDeathDelegate.Broadcast();

	FTimerHandle Handler;
	GetWorldTimerManager().SetTimer(
		Handler,
		this,
		&ABossCharater::FinishDeathAnim,
		DeathDuraton,
		false
	);

	IMainPlayerInterface* PlayerRef{

		GetWorld()->GetFirstPlayerController()->GetPawn<IMainPlayerInterface>()

	};

	if (!PlayerRef) return 0.0f;
	PlayerRef->EndLockonWithActor(this);
	return PlayAnimMontage(DeathAnim);

}

// Called every frame
void ABossCharater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossCharater::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABossCharater::DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect)
{
	EBossStats BossStats{ static_cast<EBossStats>(BlackboardComp->GetValueAsEnum(TEXT("BossStats"))) };
	if ((DetectedPawn == PawnToDetect) && BossStats == EBossStats::Idle)
	{
		BlackboardComp->SetValueAsEnum("BossStats", EBossStats::Range);
		//UE_LOG(LogTemp, Warning, TEXT("PLAYER DETECT"));
		GetComponentByClass<ULookAtPlayerComp>()->bCanLookAt = true;

	}
}

float ABossCharater::GetDamage()
{
	return statsComp->Stats[EStats::Strength];
}

void ABossCharater::OnCombat()
{
	BlackboardComp->SetValueAsEnum("BossStats", EBossStats::Melee);
}

void ABossCharater::Attack()
{
	CombatComp->RandomAttack();
}

float ABossCharater::GetAttackDuration()
{
	return CombatComp->AttackDurationTime;
}

float ABossCharater::GetMeleeRange()
{
	return statsComp->Stats[EStats::MeleeRange];
}

void ABossCharater::HandleGameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("PLAYER DEAD"));
	BlackboardComp->SetValueAsEnum("BossStats",EBossStats::GameOver);
}

void ABossCharater::FinishDeathAnim()
{
	Destroy();
}

bool ABossCharater::CanTakeDamage(AActor* Opponent)
{
	return true;
}



