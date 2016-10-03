// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TBSGrid.h"


/**
 * 
 */
static class TBSGridPathFindingHelper
{
public:

	static TMap<FIntVector, FIntVector> FindAllPossiblePaths(ATBSGrid* Grid, FIntVector Start, FIntVector Dimensions);

	static bool FindUncachedPath(ATBSGrid* Grid, FIntVector Start, FIntVector End, TArray<FIntVector> &OutPath, FIntVector Dimensions);

	static bool FindCachedPath(FIntVector End, TMap<FIntVector, FIntVector> Paths, TArray<FIntVector>& OutPath);

	static float CalculateMovementCost(PathStep Current, FIntVector NextNode);

	static float CalculateMovementCostMultiplier(PathStep Current, FIntVector NextNode);

	static TArray<FIntVector> GetNeighbourNodes(ATBSGrid* Grid, FIntVector Coordinates, FIntVector Dimensions);

	static TArray<FIntVector> GetTilesUnderFootprint(FIntVector Coordinates, FIntVector Dimensions);

	//Checks if current position has enough things to stand on
	static bool IsSupportedPosition(TArray<FIntVector> Tiles);

	static bool IsAccessiblePosition(ATBSGrid* Grid, TArray<FIntVector> Tiles, FIntVector Dimensions);

	static bool IsPossiblePath(ATBSGrid* Grid, FIntVector Start, FIntVector End, FIntVector Dimensions);

	static bool IsCloseEnough(FIntVector Start, FIntVector End);

	static bool IsAccessibleEnd(ATBSGrid* Grid, FIntVector End, FIntVector Dimensions);


private:
};

