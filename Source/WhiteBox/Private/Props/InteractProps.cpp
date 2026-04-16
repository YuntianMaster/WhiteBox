// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/InteractProps.h"
#include "Player/PlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Travel/CollectionComp.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AInteractProps::AInteractProps()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//check Area
	SphereActiveArea = CreateDefaultSubobject<USphereComponent>(FName("SphereActiveArea"));
	SphereActiveArea->SetGenerateOverlapEvents(true);
	SphereActiveArea->SetCollisionProfileName("Trigger", true);
	//overlap
	//UI
	UIWidget = CreateDefaultSubobject<UWidgetComponent>(FName("PickUpUI"));
	UIWidget->SetupAttachment(SphereActiveArea);
	UIWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetInstance(TEXT("/Game/WhiteBox/UI/WBP_PICKUP"));
	if (WidgetInstance.Succeeded()) {
		WidgetClass = WidgetInstance.Class;
	}

}

// Called when the game starts or when spawned
void AInteractProps::BeginPlay()
{
	Super::BeginPlay();
	//SphereActiveArea->SetSphereRadius(checkRadius);
	/*UUserWidget* WidgetUI = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	if (WidgetUI) {
		UIWidget->SetWidget(WidgetUI);
	}*/
	UIWidget->SetVisibility(false);

	FScriptDelegate OnBeginOverlapDelegate;
	OnBeginOverlapDelegate.BindUFunction(this, "PopUpUI");
	SphereActiveArea->OnComponentBeginOverlap.Add(OnBeginOverlapDelegate);
	FScriptDelegate OnEndOverlapDelegate;
	OnEndOverlapDelegate.BindUFunction(this, "PopDownUI");
	SphereActiveArea->OnComponentEndOverlap.Add(OnEndOverlapDelegate);
}

// Called every frame
void AInteractProps::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bDebugMode)
		UKismetSystemLibrary::DrawDebugSphere(GetWorld(), GetActorLocation(), checkRadius);
}


void AInteractProps::InteractHandle()
{
	UE_LOG(LogTemp, Warning, TEXT("blind!"));
}

void AInteractProps::PopUpUI(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Tags.Contains("Player"))
	{
		UIWidget->SetVisibility(true);
		bcanInteract = true;
		APlayerCharacter* ChaRef = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (ChaRef)
			ChaRef->GetComponentByClass<UCollectionComp>()->OnPlayerInteractDelegate.AddUniqueDynamic(this, &AInteractProps::InteractHandle);
	}
}

void AInteractProps::PopDownUI(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Tags.Contains("Player"))
	{
		UIWidget->SetVisibility(false);
		bcanInteract = false;
		if (!GetWorld()) return;
		if (!GetWorld()->GetFirstPlayerController()) return;
		APlayerCharacter* ChaRef = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (ChaRef)
			ChaRef->GetComponentByClass<UCollectionComp>()->OnPlayerInteractDelegate.RemoveDynamic(this, &AInteractProps::InteractHandle);
	}
}

