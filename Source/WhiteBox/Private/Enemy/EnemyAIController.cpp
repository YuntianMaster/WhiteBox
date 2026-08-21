// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "Enemy/EEnemyStats.h"
#include "Enum/ECombatEnum.h"
#include "Enemy/EnemyCharacter.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AttrubuteSet/CombatAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISense_Sight.h"
#include "Components/StateTreeAIComponent.h"

AEnemyAIController::AEnemyAIController()
{
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
	StateTreeAIComp = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeComp"));
	//AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdatedHandle);
	//OnHP75Delegate.AddDynamic(this, &AEnemyAIController::OnHP75Handle);
	
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdatedHandle);
	// BP 资源里可能序列化了旧的 AddDynamic/Assign 绑定，构造里注释掉不会清掉，必须在反序列化之后 Remove。
	/*if (AIPerceptionComp)
	{
		AIPerceptionComp->OnTargetPerceptionUpdated.RemoveDynamic(this, &AEnemyAIController::OnPerceptionUpdatedHandle);
	}*/
}

void AEnemyAIController::isSeeingPlayer(const FAIStimulus& Stimulus)
{
	TSubclassOf<UAISense> SenseClass = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);
	if (SenseClass != UAISense_Sight::StaticClass())return;
	if (!Stimulus.WasSuccessfullySensed())return;
	//UBlackboardComponent* BlackBoardComp  = GetBlackboardComponent();
	//BlackBoardComp->SetValueAsEnum("EnemyStats", EEnemyStats::Alarm);

}

void AEnemyAIController::OnPerceptionUpdatedHandle(AActor* Actor, FAIStimulus Stimulus)
{
	
	TSubclassOf<UAISense> SenseClass = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);

	if (SenseClass == UAISense_Sight::StaticClass())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			GetWorld()->GetTimerManager().ClearTimer(LostSennStimulusHandle);
			FStateTreeEvent TreeBattleEvent;
			TreeBattleEvent.Tag = FGameplayTag::RequestGameplayTag("Event.StateTree.Battle");
			StateTreeAIComp->SendStateTreeEvent(TreeBattleEvent);

			UE_LOG(LogTemp, Warning, TEXT("Battle"));
		}

		else  {
			GetWorld()->GetTimerManager().SetTimer(
				LostSennStimulusHandle,
				this,
				&AEnemyAIController::LostSeenStimulusHandler,
				2.5f,
				false
			);
		}

	}


	EnemyTargetActor = Actor;
	//UBlackboardComponent* BlackBoardComp = GetBlackboardComponent();
	//BlackBoardComp->SetValueAsObject("TargetActor", Actor);
	
	
	
}

void AEnemyAIController::LostSeenStimulusHandler()
{
	UE_LOG(LogTemp, Warning, TEXT("Partol"));
	FStateTreeEvent TreePartolEvent;
	TreePartolEvent.Tag = FGameplayTag::RequestGameplayTag("Event.StateTree.Patrol");
	StateTreeAIComp->SendStateTreeEvent(TreePartolEvent);
}

//void AEnemyAIController::InitialEnemyBlackBoard()
//{
//	UBlackboardComponent* BlackBoardComp = GetBlackboardComponent();
//	if (!BlackBoardComp)
//	{
//		return;
//	}
//
//	APawn* ControlledPawn = GetPawn();
//	if (!ControlledPawn)
//	{
//		return;
//	}
//
//	UAbilitySystemComponent* ASC = ControlledPawn->FindComponentByClass<UAbilitySystemComponent>();
//	if (!ASC)
//	{
//		return;
//	}
//
//	const UAttributeSet* AttributeSet = ASC->GetAttributeSet(UCombatAttributeSet::StaticClass());
//	const UCombatAttributeSet* CombatAttributeSet = Cast<UCombatAttributeSet>(AttributeSet);
//	if (!CombatAttributeSet)
//	{
//		return;
//	}
//
//	const float MeleeRangeDistance = CombatAttributeSet->GetMeleeAttackRange();
//	if (MeleeRangeDistance <= 0.f)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("MeleeAttackRange is zero or unset"));
//		return;
//	}
//	BlackBoardComp->SetValueAsFloat(TEXT("MeleeRangeDistance"), MeleeRangeDistance);
//	BlackBoardComp->SetValueAsFloat(TEXT("StrafeRangeDistance"), CombatAttributeSet->GetStrafeRange());
//	BlackBoardComp->SetValueAsFloat(TEXT("SafeDistance"), 150.f);
//
//	
//}
//
//void AEnemyAIController::OnHP75Handle()
//{
//	UE_LOG(LogTemp, Warning, TEXT("HP75!!!"));
//
//	UBlackboardComponent* BlackBoardComp = GetBlackboardComponent();
//	BlackBoardComp->SetValueAsEnum("EnemyStats", EEnemyStats::P1);
//}

//void AEnemyAIController::SetEnemyCombatSpeedsByFloat(float CustomCombatSpeed)
//{
//
//	APawn* ControlledPawn = GetPawn();
//	if (!ControlledPawn)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Controlled Pawn does not exist!"));
//		return;
//	}
//
//	ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn);
//	if (!ControlledCharacter)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Controlled Pawn is not a Character!"));
//		return;
//	}
//	ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = CustomCombatSpeed;
//	
//}
//
//void AEnemyAIController::SetEnemyCombatSpeedsByEnum(ECombatSpeeds CombatSpeedEnum)
//{
//
//	APawn* ControlledPawn = GetPawn();
//	if (!ControlledPawn)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Controlled Pawn does not exist!"));
//		return;
//	}
//	AEnemyCharacter* ControlledCharacter = Cast<AEnemyCharacter>(ControlledPawn);
//	if (!ControlledCharacter)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Controlled Pawn is not a Character!"));
//		return;
//	}
//	UCombatAttributeSet* CombatAttributeSet = ControlledCharacter->CombatAttributeSet;
//	switch (CombatSpeedEnum)
//	{
//	case ECombatSpeeds::WalkSpeed:
//	{
//		float walkSpeed = CombatAttributeSet->GetWalkSpeed();
//		ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
//		break;
//	}
//	case ECombatSpeeds::SprintSpeed:
//	{
//		float sprintSpeed = CombatAttributeSet->GetSprintSpeed();
//		ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = sprintSpeed;
//		break;
//	}
//	case ECombatSpeeds::StrafeSpeed:
//	{
//		float strafeSpeed = CombatAttributeSet->GetStrafeSpeed();
//		ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = strafeSpeed;
//		break;
//	}
//	default:
//		break;
//	}
//}
