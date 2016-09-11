// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "Algo/Reverse.h"
#include "TBSGridPathFinder.h"
#include "TBSGridPathFindingHelper.h"
#include "TBSGridPathWorker.h"

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
	InvalidateCache();
}

bool ATBSGridPathFinder::FindPath(FIntVector Start, FIntVector End, TArray<FIntVector> &OutPath, FIntVector Dimensions)
{
	if (HasCache())
	{
		return TBSGridPathFindingHelper::FindCachedPath(End, CachedPaths, OutPath);
	}
	else
	{
		return TBSGridPathFindingHelper::FindUncachedPath(Grid, Start, End, OutPath, Dimensions);
	}
}

void ATBSGridPathFinder::BuildCache(FIntVector Start, FIntVector Dimensions)
{
	TBSGridPathWorker::Shutdown();
	TBSGridPathWorker::JoyInit(CachedPaths, Start, Dimensions, Grid);
}

void ATBSGridPathFinder::InvalidateCache()
{
	TBSGridPathWorker::Shutdown();
	CachedPaths.Empty();
}

bool ATBSGridPathFinder::HasCache()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("elements in cached path %i"), CachedPaths.Num()));
	return CachedPaths.Num() > 0 && TBSGridPathWorker::IsThreadFinished();
}
