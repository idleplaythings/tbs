// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSProp.h"
#include "TBSGrid.generated.h"

UCLASS()
class TBS_API ATBSGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBSGrid();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
	int32 GridWidth = 0;
	int32 GridHeight = 0;
	int32 NumOfLevels = 0;

	void InitialiseGrid(int32 GridWidth, int32 GridHeight, int32 NumOfLevels);
	TArray<FIntVector> GetAccessibleNeighbours(FIntVector Coordinates);
	TArray<FIntVector> GetNeighbours(FIntVector Coordinates);

	void AddProp(FProp Prop);
	void AddUnit(FUnit* Unit);
	TMap<FIntVector, TArray<FProp>>::TConstIterator GetPropsIterator();
	TArray<FUnit*>::TIterator GetUnitIterator();
	FUnit* SelectUnit(FIntVector GameCoords);

private:
	TMap<FIntVector, TArray<FProp>> Props;
	TArray<FUnit*> Units;
	bool IsTileAccessBlockedByProp(FIntVector Coordinates, ETileSlot Slot);
};
