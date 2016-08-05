// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
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
	Frontier.HeapPush(PathStep(Start, 0), PathStepPredicate());

	TMap<FIntVector, FIntVector> CameFrom;
	CameFrom.Add(Start, FIntVector(-999, -999, -999));

	TMap<FIntVector, int32> CostSoFar;
	CostSoFar.Add(Start, 0);

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
			int32 NewCost = CostSoFar[Current.Coordinates] + 1; // Cost of moving from current to next
			if (!CostSoFar.Contains(Next) || NewCost < CostSoFar[Next])
			{
				CostSoFar.Add(Next, NewCost);
				int32 Priority = FMath::Abs(End.X - Next.X) + FMath::Abs(End.Y - Next.Y);
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

	return Path;
}
