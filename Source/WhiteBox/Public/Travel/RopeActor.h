// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Widget/SceneActorWidget.h"
#include "RopeActor.generated.h"

UCLASS()
class WHITEBOX_API ARopeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARopeActor();




	virtual bool ShouldTickIfViewportsOnly() const override
	{
		return true;   // 允许在编辑器视口 Tick
	}


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USceneActorWidget* WidgetRef;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* StartPoint;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* EndPoint;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* Rope;
	void getStartLocation();

	FVector StartLoc{};
	FVector EndLoc{};
	FRotator RopeRot{};
	
	void SetIconVisable();
	void SetIconInvisable();
		
};
