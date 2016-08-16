// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "Algo/Reverse.h"
#include "TBSGridPathFinder.h"

// Sets default values
ATBSGridPathFinder::ATBSGridPathFinder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATBSGridPathFinder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBSGridPathFinder::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATBSGridPathFinder::Initialise(ATBSGrid* InGrid)
{
	Grid = InGrid;
}

TArray<FIntVector> ATBSGridPathFinder::FindPath(FIntVector Start, FIntVector End)
{
	TArray<PathStep> Frontier;
	Frontier.HeapPush(PathStep(Start, 0.0), PathStepPredicate());

	TMap<FIntVector, FIntVector> CameFrom;
	CameFrom.Add(Start, FIntVector(-999, -999, -999));

	TMap<FIntVector, float> CostSoFar;
	CostSoFar.Add(Start, 0.0);

	bool FoundEnd = false;

	while (Frontier.Num() > 0)
	{
		PathStep Current;
		Frontier.HeapPop(Current, PathStepPredicate());

		if (Current.Coordinates == End)
		{
			FoundEnd = true;
			break;
		}

		for (auto& Next : Grid->GetAccessibleNeighbours(Current.Coordinates))
		{		
			float NewCost;

			// Diagonal movement
			if (Current.Coordinates.X != Next.X && Current.Coordinates.Y != Next.Y)
			{
				NewCost = CostSoFar[Current.Coordinates] + 1.42; // Cost of moving from current to next
			}
			// Straight movement
			else
			{
				NewCost = CostSoFar[Current.Coordinates] + 1.0; // Cost of moving from current to next
			}
			
			if (!CostSoFar.Contains(Next) || NewCost < CostSoFar[Next])
			{
				CostSoFar.Add(Next, NewCost);
				float Priority = NewCost + (float)(FMath::Abs(End.X - Next.X) + FMath::Abs(End.Y - Next.Y));
				Frontier.HeapPush(PathStep(Next, Priority), PathStepPredicate());
				CameFrom.Add(Next, Current.Coordinates);
			}
		}
	}

	FIntVector Current = End;
	TArray<FIntVector> Path;		

	do
	{
		Path.Add(Current);
		Current = CameFrom[Current];
	} while (Current != FIntVector(-999, -999, -999));

	Algo::Reverse(Path);

	return Path;
}
