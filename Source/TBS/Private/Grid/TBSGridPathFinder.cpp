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

bool ATBSGridPathFinder::FindPath(FIntVector Start, FIntVector End, TArray<FIntVector> &OutPath)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Find path (%i, %i, %i) -> (%i, %i, %i)"), Start.X, Start.Y, Start.Z, End.X, End.Y, End.Z));

	if (Grid == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Null grid")));
		return false;
	}

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

		for (auto& Next : GetNeighbourNodes(Current.Coordinates, FIntVector(2,2,4)))
		{		
			float NewCost;

			float MovementCost = calculateMovementCost(Current, Next);
			NewCost = CostSoFar[Current.Coordinates] + MovementCost;
			
			if (!CostSoFar.Contains(Next) || NewCost < CostSoFar[Next])
			{
				CostSoFar.Add(Next, NewCost);
				float Priority = NewCost + (float)(FMath::Abs(End.X - Next.X) + FMath::Abs(End.Y - Next.Y));
				Frontier.HeapPush(PathStep(Next, Priority), PathStepPredicate());
				CameFrom.Add(Next, Current.Coordinates);
			}
		}
	}

	if (!FoundEnd)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("No end found")));
		return false;
	}

	FIntVector Current = End;
	TArray<FIntVector> Path;		

	do
	{
		Path.Add(Current);
		Current = CameFrom[Current];
	} while (Current != FIntVector(-999, -999, -999));

	Algo::Reverse(Path);

	OutPath = Path;

	return true;
}

float ATBSGridPathFinder::calculateMovementCost(PathStep Current, FIntVector NextNode)
{
	float multiplier = calculateMovementCostMultiplier(Current, NextNode);
	// Tile by tile movement cost here
	return multiplier;
}

float ATBSGridPathFinder::calculateMovementCostMultiplier(PathStep Current, FIntVector NextNode)
{
	// Diagonal movement
	if (Current.Coordinates.X != NextNode.X && Current.Coordinates.Y != NextNode.Y)
	{
		return 1.42f;
	}
	// Straight movement
	else
	{
		return + 1.0f;
	}

}

TArray<FIntVector> ATBSGridPathFinder::GetNeighbourNodes(FIntVector Coordinates, FIntVector Dimensions)
{
	TArray<FIntVector> Nodes =  Grid->GetNeighbours(Coordinates);

	//Find vertical alternatives later

	Nodes = Nodes.FilterByPredicate([&](const FIntVector& Node) {
		TArray<FIntVector> Tiles = GetTilesUnderFootprint(Node, Dimensions);
		return IsSupportedPosition(Tiles) && IsAccessablePosition(Tiles, Dimensions);
	});

	return Nodes;
}

TArray<FIntVector> ATBSGridPathFinder::GetTilesUnderFootprint(FIntVector Coordinates, FIntVector Dimensions)
{
	TArray<FIntVector> Tiles;

	int32 XStep = Dimensions.X % 2 == 0 ? -5 : 0;
	int32 YStep = Dimensions.Y % 2 == 0 ? -5 : 0;

	FIntVector Origin = FIntVector(
		Coordinates.X + XStep - (FMath::CeilToInt((float) Dimensions.X / 2)*10 - 10),
		Coordinates.Y + YStep - (FMath::CeilToInt((float) Dimensions.Y / 2)*10 - 10),
		Coordinates.Z
	);

	for (int32 X = 0; X <= Dimensions.X; X = X+10)
	{
		for (int32 Y = 0; Y <= Dimensions.Y; Y = Y+10)
		{
			Tiles.Add(FIntVector(
				Origin.X + X,
				Origin.Y + Y,
				Coordinates.Z
			));
		}
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Tilenön12 (%i, %i, %i)"), Tiles[0].X, Tiles[0].Y, Tiles[0].Z));
	return Tiles;
}

/*
If unit is able to vault/climb, it is enough that one tile is walkable
Otherwise height difference of max 2 tiles is allowed withing unit footprint
*/
bool ATBSGridPathFinder::IsSupportedPosition(TArray<FIntVector> Tiles)
{
	for (auto& Tile : Tiles) 
	{
		//is walkable?
	}
	return true;
}

/*
Units height must fit in current position, and no tile can already blocked by unwalkable prop
*/
bool ATBSGridPathFinder::IsAccessablePosition(TArray<FIntVector> Tiles, FIntVector Dimensions)
{
	for (auto& Tile : Tiles)
	{
		for (int32 Z = 0; Z <= Dimensions.Z; Z++)
		{
			FIntVector Location = FIntVector(Tile.X, Tile.Y, Tile.Z + Z);
			if (! Grid->IsAccessible(Location))
			{
				return false;
			}
		}
		
	}
	return true;
}