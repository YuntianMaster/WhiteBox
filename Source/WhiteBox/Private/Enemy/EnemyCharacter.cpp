// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyCharacter.h"
#include "Enemy/TakeDownComponent.h"
#include "Widget/SceneActorWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/CollisionProfile.h"
#include "Enemy/WeaknessSystem.h"
#include "AbilitySystemComponent.h"
#include "AttrubuteSet/BasicAttributeSet.h"
#include "AttrubuteSet/CombatAttributeSet.h"
#include "Player/StatsComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TakeDownComp = CreateDefaultSubobject<UTakeDownComponent>(TEXT("TakeDownComp"));
	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComp"));
	UIComp = CreateDefaultSubobject<USceneActorWidget>(TEXT("UIComp"));
	//UIComp->SetupAttachment(GetMesh());
	SkelealComp = GetComponentByClass<USkeletalMeshComponent>();
	WeaknessComp = CreateDefaultSubobject<UWeaknessSystem>(TEXT("WeaknessSystemComp"));
	AbilitySysComp = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySysComp"));
	BasicAttributeSet = CreateDefaultSubobject<UBasicAttributeSet>(TEXT("BasicAttributeSet"));
	CombatAttributeSet = CreateDefaultSubobject<UCombatAttributeSet>(TEXT("CombatAttributeSet"));
	CharacterUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	CharacterUI->SetupAttachment(GetMesh());
		
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	/*AnimInstance = GetMesh()->GetAnimInstance();
	UIComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("FX_Head"));*/

	EndDelegate.BindUFunction(this, "OnCatchHandle");


}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::beTakeDownHandle(UAnimMontage* beTakeDownAnim)
{
	PlayAnimMontage(beTakeDownAnim);
}

void AEnemyCharacter::Catch()
{
	bIsCatchComplete = false;
	float MontagePlayLength = PlayAnimMontage(CatchAnimMontage);
	AnimInstance->Montage_SetEndDelegate(EndDelegate);

	EObjectTypeQuery PawnObjectType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(PawnObjectType);
	TArray<AActor*> IngoreActor;
	IngoreActor.Add(this);
	TArray<FHitResult> OutHit;
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 200.f;
	UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		Start,
		End,
		35.f,
		ObjectTypes,
		false,
		IngoreActor,
		EDrawDebugTrace::ForDuration,
		OutHit,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.f
	);

	UUserWidget* addUI =  CreateWidget(GetWorld(), CatchUI);
	addUI->AddToViewport();
}

void AEnemyCharacter::OnCatchHandle()
{
	bIsCatchComplete = true;
}






