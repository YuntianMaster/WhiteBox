// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/Generators/EnvQueryGenerator_ProjectedPoints.h"
#include "EnvQueryGenerator_ConeBehindQuerier.generated.h"

class UEnvQueryContext;
/**
 * EQS generator: samples points in a horizontal cone opening behind the Querier (opposite of Forward).
 * Similar idea to the stock "cone in front of querier", but the axis is -Forward, rotated around actor Up.
 */
UCLASS(meta = (DisplayName = "Querier后方锥形"))
class WHITEBOX_API UEnvQueryGenerator_ConeBehindQuerier : public UEnvQueryGenerator_ProjectedPoints
{
	GENERATED_BODY()

public:
	UEnvQueryGenerator_ConeBehindQuerier();

	virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;

protected:
	/** Center(s) for generation (same idea as Simple Grid “Generate Around”: Querier, Target context, etc.). */
	UPROPERTY(EditAnywhere, Category = Generator)
	TSubclassOf<UEnvQueryContext> GenerateAround;

	/** Total horizontal arc width in degrees (e.g. 90 => +/- 45° from straight behind). */
	UPROPERTY(EditAnywhere, Category = ConeBehind)
	float ConeArcDegrees = 90.f;

	/** Angular step between rays on the horizontal arc (degrees). Must be > 0. */
	UPROPERTY(EditAnywhere, Category = ConeBehind)
	float AngleStepDegrees = 10.f;

	/** First distance along a ray from the querier origin (uu). */
	UPROPERTY(EditAnywhere, Category = ConeBehind)
	float MinDistance = 100.f;

	/** Last distance along a ray from the querier origin (uu). */
	UPROPERTY(EditAnywhere, Category = ConeBehind)
	float MaxDistance = 500.f;

	/** Spacing between consecutive samples on the same ray. Must be > 0. */
	UPROPERTY(EditAnywhere, Category = ConeBehind)
	float DistanceStep = 100.f;

	/** Added to querier location along actor up before generating (uu). */
	UPROPERTY(EditAnywhere, Category = ConeBehind)
	float ItemHeightOffset = 0.f;

private:
	static AActor* ResolveQuerierActor(UObject* QueryOwner);
};
