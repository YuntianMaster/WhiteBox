// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact.h"
#include "InteractProps.generated.h"

UCLASS()
class WHITEBOX_API AInteractProps : public AActor, public IInteract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractProps();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	virtual void InteractHandle() override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USphereComponent* SphereActiveArea;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float checkRadius{ 32.f };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UWidgetComponent* UIWidget;
	UFUNCTION()
	virtual void PopUpUI(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult)override;
	UFUNCTION()
	virtual void PopDownUI(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;
	TSubclassOf<UUserWidget> WidgetClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bDebugMode{ true };
	bool bcanInteract{ false };
	

};
