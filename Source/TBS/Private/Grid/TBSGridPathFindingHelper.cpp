// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSGridPathFindingHelper.h"

TMap<FIntVector, FIntVector> TBSGridPathFindingHelper::FindAllPossiblePaths(ATBSGrid* Grid, FIntVector Start, FIntVector Dimensions)
{
	TArray<PathStep> Frontier;
	Frontier.HeapPush(PathStep(Start, 0.0), PathStepPredicate());

	TMap<FIntVector, FIntVector> CameFrom;
	CameFrom.Add(Start, FIntVector::NoneValue);

	TMap<FIntVector, float> CostSoFar;
	CostSoFar.Add(Start, 0.0);

	while (Frontier.Num() > 0)
	{
		PathStep Current;
		Frontier.HeapPop(Current, PathStepPredicate());

		for (auto& Next : GetNeighbourNodes(Grid, Current.Coordinates, Dimensions))
		{
			float NewCost;

			float MovementCost = CalculateMovementCost(Current, Next);
			NewCost = CostSoFar[Current.Coordinates] + MovementCost;

			// Limit path finding to some arbitrary cost now
			if (NewCost < 35)
			{
				if (!CostSoFar.Contains(Next) || NewCost < CostSoFar[Next])
				{
					CostSoFar.Add(Next, NewCost);
					float Priority = 1.0f;
					Frontier.HeapPush(PathStep(Next, Priority), PathStepPredicate());
					CameFrom.Add(Next, Current.Coordinates);
				}
			}
		}
	}

	return CameFrom;
}

bool TBSGridPathFindingHelper::FindUncachedPath(ATBSGrid* Grid, FIntVector Start, FIntVector End, TArray<FIntVector>& OutPath, FIntVector Dimensions)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Find path (%i, %i, %i) -> (%i, %i, %i)"), Start.X, Start.Y, Start.Z, End.X, End.Y, End.Z));

	if (!IsPossiblePath(Grid, Start, End, Dimensions))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Path impossible to begin with")));
		return false;
	}

	if (Grid == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Null grid")));
		return false;
	}

	TArray<PathStep> Frontier;
	Frontier.HeapPush(PathStep(Start, 0.0), PathStepPredicate());

	TMap<FIntVector, FIntVector> CameFrom;
	CameFrom.Add(Start, FIntVector::NoneValue);

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

		for (auto& Next : GetNeighbourNodes(Grid, Current.Coordinates, Dimensions))
		{
			float NewCost;

			float MovementCost = CalculateMovementCost(Current, Next);
			NewCost = CostSoFar[Current.Coordinates] + MovementCost;

			// Limit path finding to some arbitrary cost now
			if (NewCost < 35)
			{
				if (!CostSoFar.Contains(Next) || NewCost < CostSoFar[Next])
				{
					CostSoFar.Add(Next, NewCost);
					float Priority = NewCost + (float)(FMath::Abs(End.X - Next.X) + FMath::Abs(End.Y - Next.Y));
					Frontier.HeapPush(PathStep(Next, Priority), PathStepPredicate());
					CameFrom.Add(Next, Current.Coordinates);
				}
			}
		}
	}

	if (!FoundEnd)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("No end found")));
		return false;
	}

	return FindCachedPath(End, CameFrom, OutPath);
}

bool TBSGridPathFindingHelper::FindCachedPath(FIntVector End, TMap<FIntVector, FIntVector> Paths, TArray<FIntVector>& OutPath)
{
	if (!Paths.Contains(End))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("En mie mitään löyä")));
		return false;
	}

	FIntVector Current = End;
	TArray<FIntVector> Path;

	do
	{
		Path.Add(Current);
		Current = Paths[Current];
	} while (Current != FIntVector::NoneValue);

	Algo::Reverse(Path);

	OutPath = Path;

	return true;
}

float TBSGridPathFindingHelper::CalculateMovementCost(PathStep Current, FIntVector NextNode)
{
	float multiplier = CalculateMovementCostMultiplier(Current, NextNode);
	// Tile by tile movement cost here
	return multiplier;
}

float TBSGridPathFindingHelper::CalculateMovementCostMultiplier(PathStep Current, FIntVector NextNode)
{
	// Diagonal movement
	if (Current.Coordinates.X != NextNode.X && Current.Coordinates.Y != NextNode.Y)
	{
		return 1.42f;
	}
	// Straight movement
	else
	{
		return +1.0f;
	}

}

TArray<FIntVector> TBSGridPathFindingHelper::GetNeighbourNodes(ATBSGrid* Grid, FIntVector Coordinates, FIntVector Dimensions)
{
	TArray<FIntVector> Nodes = Grid->GetNeighbours(Coordinates);

	//Find vertical alternatives later

	Nodes = Nodes.FilterByPredicate([&](const FIntVector& Node) {
		TArray<FIntVector> Tiles = GetTilesUnderFootprint(Node, Dimensions);
		return IsSupportedPosition(Tiles) && IsAccessablePosition(Grid, Tiles, Dimensions);
	});

	return Nodes;
}

TArray<FIntVector> TBSGridPathFindingHelper::GetTilesUnderFootprint(FIntVector Coordinates, FIntVector Dimensions)
{
	TArray<FIntVector> Tiles;

	int32 XStep = Dimensions.X % 2 == 0 ? -5 : 0;
	int32 YStep = Dimensions.Y % 2 == 0 ? -5 : 0;

	FIntVector Origin = FIntVector(
		Coordinates.X + XStep - (FMath::CeilToInt((float)Dimensions.X / 2) * 10 - 10),
		Coordinates.Y + YStep - (FMath::CeilToInt((float)Dimensions.Y / 2) * 10 - 10),
		Coordinates.Z
	);

	for (int32 X = 0; X <= Dimensions.X * 10; X = X + 10)
	{
		for (int32 Y = 0; Y <= Dimensions.Y * 10; Y = Y + 10)
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
bool TBSGridPathFindingHelper::IsSupportedPosition(TArray<FIntVector> Tiles)
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
bool TBSGridPathFindingHelper::IsAccessablePosition(ATBSGrid* Grid, TArray<FIntVector> Tiles, FIntVector Dimensions)
{
	FIntVector Location = FIntVector(0, 0, 0);
	for (auto& Tile : Tiles)
	{
		for (int32 Z = 0; Z <= Dimensions.Z; Z++)
		{
			Location.X = Tile.X;
			Location.Y = Tile.Y;
			Location.Z = Tile.Z + Z;

			if (!Grid->IsAccessible(Location))
			{
				return false;
			}
		}

	}
	return true;
}

bool TBSGridPathFindingHelper::IsPossiblePath(ATBSGrid* Grid, FIntVector Start, FIntVector End, FIntVector Dimensions)
{
	return IsCloseEnough(Start, End) && IsAccessableEnd(Grid, End, Dimensions);
}

bool TBSGridPathFindingHelper::IsCloseEnough(FIntVector Start, FIntVector End)
{
	int32 Size = (Start - End).Size();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Distance to target %i"), Size));
	//Magic number must be calculated from units movement speed/ability
	return Size < 350;
}

bool TBSGridPathFindingHelper::IsAccessableEnd(ATBSGrid* Grid, FIntVector End, FIntVector Dimensions)
{
	TArray<FIntVector> Tiles = GetTilesUnderFootprint(End, Dimensions);
	return IsAccessablePosition(Grid, Tiles, Dimensions);
}