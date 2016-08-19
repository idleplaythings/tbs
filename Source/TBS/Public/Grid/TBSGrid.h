// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSProp.h"
#include "TBSUnit.h"
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
	//virtual void Tick( float DeltaSeconds ) override;
	
	UPROPERTY(Replicated)
	FIntVector GridDimensions;

	UPROPERTY(Replicated)
	TArray<ATBSUnit*> Units;

	void InitialiseGrid(FIntVector GridDimensions);
	TArray<FIntVector> GetAccessibleNeighbours(FIntVector Coordinates);
	TArray<FIntVector> GetNeighbours(FIntVector Coordinates);

	void AddProp(FProp Prop);
	void AddUnit(ATBSUnit* Unit);
	TMap<FIntVector, TArray<FProp>>::TConstIterator GetPropsIterator();
	TArray<ATBSUnit*>::TIterator GetUnitIterator();
	ATBSUnit* SelectUnit(FIntVector GameCoords);

private:
	TMap<FIntVector, TArray<FProp>> Props;
	
	bool IsTileAccessBlockedByProp(FIntVector Coordinates, ETileSlot Slot);
};
