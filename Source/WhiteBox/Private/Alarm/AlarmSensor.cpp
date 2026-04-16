// Fill out your copyright notice in the Description page of Project Settings.


#include "Alarm/AlarmSensor.h"
#include "Alarm/AlarmActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AAlarmSensor::AAlarmSensor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAlarmSensor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAlarmSensor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAlarmSensor::RotHead()
{
	TArray<USceneComponent*> AllComponents;
	GetComponents(AllComponents);

	for (USceneComponent* comp : AllComponents) {

		UE_LOG(LogTemp, Warning, TEXT("Raw: %s"), *comp->GetName());
		if (comp && comp->GetName() == TEXT("Head")) {

			HeadComp = comp;
			break;
		}

	}

	FTimerHandle HeadRoator;

	GetWorld()->GetTimerManager().SetTimer(
		HeadRoator,
		this,
		&AAlarmSensor::UpdateRotation,
		0.02f,
		true
		);

}

void AAlarmSensor::UpdateRotation()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	float Current = GetYaw();
	SetYaw(FMath::FInterpConstantTo(Current, TargetRot, DeltaTime, speed));
	

	//UE_LOG(LogTemp, Warning, TEXT("Raw: %f"), Current);
	if (FMath::IsNearlyEqual(Current, TargetRot, 0.01f))
	{

		if (TargetRot == RightLimit)
			TargetRot = LeftLimit;
		else
			TargetRot = RightLimit;


	}
}

float AAlarmSensor::GetYaw()
{
	 return HeadComp->GetComponentRotation().Yaw;
}

void AAlarmSensor::SetYaw(float Yaw) {
	FRotator NewRot{ HeadComp->GetComponentRotation() };
	NewRot.Yaw = Yaw;
	HeadComp->SetWorldRotation(NewRot);
}

void AAlarmSensor::HandleStartAlarm(AActor* Player)
{
	if (!Player->ActorHasTag(FName("Player"))) return;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAlarmActor::StaticClass(), FoundActors);
	TArray<AAlarmActor*> Alarms;
	for (AActor* Actor : FoundActors)
	{
		if (AAlarmActor* Alarm = Cast<AAlarmActor>(Actor))
		{
			Alarms.Add(Alarm);
		}
	}
	if (Alarms.Num() == 0) return;
	for (AAlarmActor* al : Alarms) {
		al->StartAlarm();

		FTimerHandle EndAlarmHandler;

		GetWorldTimerManager().SetTimer(
			EndAlarmHandler,
			al,
			&AAlarmActor::EndAlarm,
			5.0f,
			false
		);


	}
}
