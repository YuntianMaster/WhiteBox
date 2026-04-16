// Fill out your copyright notice in the Description page of Project Settings.


#include "Travel/RopeActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Widget/SceneActorWidget.h"

// Sets default values
ARopeActor::ARopeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;


	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRoot;
	StartPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StartComponent"));
	StartPoint->SetupAttachment(RootComponent);
	EndPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EndComponent"));
	EndPoint->SetupAttachment(RootComponent);
	Rope = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RopeLine"));
	Rope->SetupAttachment(RootComponent);
	WidgetRef = CreateDefaultSubobject<USceneActorWidget>(TEXT("WidgetComp"));
	WidgetRef->SetupAttachment(StartPoint);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));

	if (CubeMesh.Succeeded())
	{
		StartPoint->SetStaticMesh(CubeMesh.Object);
		EndPoint->SetStaticMesh(CubeMesh.Object);
		Rope->SetStaticMesh(CubeMesh.Object);
	}

	

}

// Called when the game starts or when spawned
void ARopeActor::BeginPlay()
{
	Super::BeginPlay();
	StartLoc = StartPoint->K2_GetComponentLocation();
	EndLoc = EndPoint->K2_GetComponentLocation();
	
	RopeRot = UKismetMathLibrary::FindLookAtRotation(StartLoc, EndLoc);
	
}

// Called every frame
void ARopeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!StartPoint || !EndPoint) return;
	if (GetWorld() && GetWorld()->WorldType == EWorldType::Editor)
	{
		StartLoc = StartPoint->K2_GetComponentLocation();
		EndLoc =  EndPoint->K2_GetComponentLocation();
		FVector Center{ (EndLoc + StartLoc) / 2 };
		RopeRot = UKismetMathLibrary::FindLookAtRotation(StartLoc,EndLoc);
		double Distance{ FVector::Distance(StartLoc,EndLoc) };
		Rope->SetWorldScale3D(FVector{ Distance/100 ,0.1f,0.1f });
		Rope->SetWorldRotation(RopeRot);
		Rope->SetWorldLocation(Center);
	}
}

void ARopeActor::getStartLocation()
{
}

void ARopeActor::SetIconVisable()
{
	WidgetRef->SetVisibility(true);
}


void ARopeActor::SetIconInvisable()
{
	WidgetRef->SetVisibility(false);
}
