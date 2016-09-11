// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSGrid.h"
#include "TBSGridPathFinder.generated.h"

UCLASS()
class TBS_API ATBSGridPathFinder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBSGridPathFinder();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Initialise(ATBSGrid* InGrid);	

	bool FindPath(FIntVector Start, FIntVector End, TArray<FIntVector> &OutPath, FIntVector Dimensions);

	void BuildCache(FIntVector Start, FIntVector Dimensions);

	void InvalidateCache();

private:

	ATBSGrid* Grid;
	TMap<FIntVector, FIntVector> CachedPaths;

	bool HasCache();
};
