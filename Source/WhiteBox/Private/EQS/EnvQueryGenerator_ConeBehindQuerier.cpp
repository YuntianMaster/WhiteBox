// Fill out your copyright notice in the Description page of Project Settings.

#include "EQS/EnvQueryGenerator_ConeBehindQuerier.h"

#include "AI/Navigation/NavigationTypes.h"
#include "AIController.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/Pawn.h"

UEnvQueryGenerator_ConeBehindQuerier::UEnvQueryGenerator_ConeBehindQuerier()
{
	GenerateAround = UEnvQueryContext_Querier::StaticClass();

	ConeArcDegrees = 90.f;
	AngleStepDegrees = 10.f;
	MinDistance = 100.f;
	MaxDistance = 500.f;
	DistanceStep = 100.f;
	ItemHeightOffset = 0.f;
}

FText UEnvQueryGenerator_ConeBehindQuerier::GetDescriptionTitle() const
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("Title"), NSLOCTEXT("EQS", "ConeBehindShortTitle", "Querier后方锥形"));
	Args.Add(TEXT("Context"), UEnvQueryTypes::DescribeContext(GenerateAround));
	return FText::Format(NSLOCTEXT("EQS", "ConeBehindFullTitle", "{Title}: {Context}"), Args);
}

FText UEnvQueryGenerator_ConeBehindQuerier::GetDescriptionDetails() const
{
	return FText::Format(
		NSLOCTEXT("EQS", "ConeBehindDetails", "Arc:{0}°, behind Querier, distances {1}-{2}."),
		FText::AsNumber(ConeArcDegrees),
		FText::AsNumber(MinDistance),
		FText::AsNumber(MaxDistance));
}

AActor* UEnvQueryGenerator_ConeBehindQuerier::ResolveQuerierActor(UObject* QueryOwner)
{
	if (!QueryOwner)
	{
		return nullptr;
	}

	if (APawn* Pawn = Cast<APawn>(QueryOwner))
	{
		return Pawn;
	}

	if (AAIController* AI = Cast<AAIController>(QueryOwner))
	{
		return AI->GetPawn();
	}

	return Cast<AActor>(QueryOwner);
}

void UEnvQueryGenerator_ConeBehindQuerier::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
	if (!GenerateAround)
	{
		return;
	}

	if (AngleStepDegrees <= KINDA_SMALL_NUMBER || DistanceStep <= KINDA_SMALL_NUMBER)
	{
		return;
	}

	if (MinDistance > MaxDistance)
	{
		return;
	}

	UObject* Owner = QueryInstance.Owner.Get();
	AActor* Querier = ResolveQuerierActor(Owner);
	if (!Querier)
	{
		return;
	}

	TArray<FVector> ContextLocations;
	QueryInstance.PrepareContext(GenerateAround, ContextLocations);
	if (ContextLocations.Num() == 0)
	{
		return;
	}

	const FVector Backward = (-Querier->GetActorForwardVector()).GetSafeNormal();
	const FVector AxisUp = Querier->GetActorUpVector().GetSafeNormal();

	const float HalfArc = 0.5f * ConeArcDegrees;

	TArray<FNavLocation> ItemCandidates;
	ItemCandidates.Reserve(128 * FMath::Max(1, ContextLocations.Num()));

	constexpr int32 MaxPoints = 8192;
	int32 GeneratedCount = 0;
	bool bCapReached = false;

	for (int32 ContextIndex = 0; !bCapReached && ContextIndex < ContextLocations.Num(); ++ContextIndex)
	{
		const FVector Origin =
			ContextLocations[ContextIndex] + Querier->GetActorUpVector() * ItemHeightOffset;

		for (float AngleDeg = -HalfArc; !bCapReached && AngleDeg <= HalfArc + KINDA_SMALL_NUMBER; AngleDeg += AngleStepDegrees)
		{
			const FVector Dir = Backward.RotateAngleAxis(AngleDeg, AxisUp).GetSafeNormal();

			for (float Dist = MinDistance; Dist <= MaxDistance + KINDA_SMALL_NUMBER; Dist += DistanceStep)
			{
				ItemCandidates.Add(FNavLocation(Origin + Dir * Dist));

				if (++GeneratedCount >= MaxPoints)
				{
					bCapReached = true;
					break;
				}
			}
		}
	}

	ProjectAndFilterNavPoints(ItemCandidates, QueryInstance);
	StoreNavPoints(ItemCandidates, QueryInstance);
}
