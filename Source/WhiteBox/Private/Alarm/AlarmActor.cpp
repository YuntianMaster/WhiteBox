// Fill out your copyright notice in the Description page of Project Settings.


#include "Alarm/AlarmActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/RotatingMovementComponent.h"

// Sets default values
AAlarmActor::AAlarmActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAlarmActor::BeginPlay()
{
	Super::BeginPlay();
	RoatatingMoveComp = GetComponentByClass<URotatingMovementComponent>();
	TArray<AActor*> AllComp;
	GetComponents<USpotLightComponent*>(AlarmLights);
}

// Called every frame
void AAlarmActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}



void AAlarmActor::StartAlarm()
{
	RoatatingMoveComp->RotationRate = FRotator{ 0.0f, RotationSpeed,0.0f};
	for (USpotLightComponent* Lights : AlarmLights)
	{
		Lights->SetVisibility(true);
	}
	if (AlarmAudioComponent != nullptr) return;
	AlarmAudioComponent = UGameplayStatics::SpawnSoundAtLocation(
		GetWorld(),
		AudioSource,
		GetActorLocation()
	);
}

void AAlarmActor::EndAlarm()
{
	if (AlarmAudioComponent != nullptr)
		AlarmAudioComponent->FadeOut(2.0f,0.0f);
	for (USpotLightComponent* Lights : AlarmLights)
	{
		Lights->SetVisibility(false);
	}
	RoatatingMoveComp->RotationRate = FRotator{ 0.0f, 0.0f, 0.0f, };
	AlarmAudioComponent = nullptr;


}

