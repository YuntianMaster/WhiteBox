// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Action/BTT_ChargeAttack.h"
#include "AIController.h"
#include "Enemy/BossCharater.h"
#include "Animation/BossAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interface/Fighter.h"
#include "GameFramework/CharacterMovementComponent.h"



UBTT_ChargeAttack::UBTT_ChargeAttack() {
	bNotifyTick = true;
	FMoveCompleteDelegate.BindUFunction(this, "HandleMoveCompleted");
}

void UBTT_ChargeAttack::ChargeAtPlayer()
{
	//UE_LOG(LogTemp, Warning, TEXT("START %f"), WalkMovementSpeed);
	APawn* PlayerPawn{
		GetWorld()->GetFirstPlayerController()->GetPawn()
	};
	FVector PlayerLocation{ PlayerPawn->GetActorLocation() }; 
	FAIMoveRequest MoveRequest{ PlayerLocation };
	MoveRequest.SetUsePathfinding(true);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
	ControllerRef->MoveTo(MoveRequest);
	ControllerRef->SetFocus(PlayerPawn);
	ControllerRef->ReceiveMoveCompleted.AddUnique(FMoveCompleteDelegate);
	//UE_LOG(LogTemp, Warning, TEXT("ACTOR LOCATION %f"), PlayerLocation.X);
	CharacterRef->GetCharacterMovement()->MaxWalkSpeed = ChargeMovementSpeed;
}

void UBTT_ChargeAttack::HandleMoveCompleted()
{	
	
	
	FTimerHandle AttackTimeHandler;
	ControllerRef->GetWorldTimerManager().SetTimer(
		AttackTimeHandler,
		this,
		&UBTT_ChargeAttack::FinishAttackTask,
		2.0f,
		false
	);
}

void UBTT_ChargeAttack::FinishAttackTask()
{
	/*FString StatusName = UEnum::GetValueAsString(ControllerRef->GetMoveStatus());*/
	//UE_LOG(LogTemp, Warning, TEXT("FINISH!!!"));
	UBossAnimInstance* AnimRef{ Cast<UBossAnimInstance>(ControllerRef->GetPawn<ABossCharater>()->GetMesh()->GetAnimInstance()) };
	if (!IsValid(AnimRef)) return;
	AnimRef->bIsCharging = false;	
	FTimerHandle AttackTimeHandler;
	ControllerRef->GetWorldTimerManager().SetTimer(
		AttackTimeHandler,
		this,
		&UBTT_ChargeAttack::HandleAttackFinished,
		2.4f,
		false
	);

}

void UBTT_ChargeAttack::HandleAttackFinished()
{
	bIsFinished = true;
	
}

void UBTT_ChargeAttack::ChangeAttackHandle()
{
	UE_LOG(LogTemp, Warning, TEXT("1111"));
	bAttackIsFinish = true;
	
}

void UBTT_ChargeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	AAIController* AIRef{ OwnerComp.GetAIOwner()};
	IFighter* FighterRef{ Cast<IFighter>(AIRef->GetCharacter()) };

	bool isReadycharge{ OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("isReadyToCharge"))};
	
	if (isReadycharge) {
		ChargeAtPlayer();
	}
	float Distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance"));

	if (Distance < 200.f) {

		AIRef->StopMovement();
		FinishAttackTask();

	}
	if (!bIsFinished) return;

	UE_LOG(LogTemp, Warning, TEXT("END %f"), WalkMovementSpeed);
	CharacterRef->GetCharacterMovement()->MaxWalkSpeed = WalkMovementSpeed;
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BossStats"), EBossStats::Melee);
	ControllerRef->ReceiveMoveCompleted.Remove(FMoveCompleteDelegate);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);



}




EBTNodeResult::Type UBTT_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{


	ControllerRef = OwnerComp.GetAIOwner();
	CharacterRef = OwnerComp.GetAIOwner()->GetPawn<ABossCharater>();
	WalkMovementSpeed = CharacterRef->GetCharacterMovement()->GetMaxSpeed();
	//UE_LOG(LogTemp, Warning, TEXT("in %f"), WalkMovementSpeed);
	bIsFinished = false;
	bAttackIsFinish = false;
	UBossAnimInstance* AnimRef{ 
		Cast<UBossAnimInstance>(OwnerComp.GetAIOwner()->GetPawn<ABossCharater>()->GetMesh()->GetAnimInstance())};
	BlackBoardComp = OwnerComp.GetBlackboardComponent();
	BlackBoardComp->SetValueAsBool(TEXT("isReadyToCharge"), false);
	AnimRef->bIsCharging = true;
	return EBTNodeResult::Type::InProgress;
}
