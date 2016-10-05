// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSProp.h"
#include "TBSUnit.h"
#include "TBSGrid.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorNoLongerVisible, int32, TeamNumber, AActor*, Actor);

UCLASS()
class TBS_API ATBSGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBSGrid();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
		
	UPROPERTY(Replicated)
	FIntVector GridDimensions;

	uint32 PropCount();
	uint32 NumOfProps;

	uint32 NextPropId = 1;

	UPROPERTY(Replicated)
	TArray<ATBSUnit*> Units;

	UFUNCTION(NetMulticast, Reliable)
	void ReindexProps();

	UFUNCTION(NetMulticast, Reliable)
	void ReindexUnits();

	TArray<ATBSUnit*> GetUnitsByPlayer(int32 PlayerNumber);

	bool GetLineOfFireBetweenUnits(ATBSUnit* UnitA, ATBSUnit* UnitB, TArray<FIntVector> &OutTrace);
	bool CanDrawLineOfFire(FIntVector Start, FIntVector End, TArray<FIntVector> &OutTrace);

	TArray<FIntVector> Trace(FIntVector Start, FIntVector End);

	void InitialiseGrid(FIntVector GridDimensions);
	TArray<FIntVector> GetAccessibleNeighbours(FIntVector Coordinates);
	TArray<FIntVector> GetNeighbours(FIntVector Coordinates);
	bool IsAccessible(FIntVector Coordinates);

	void AddProp(FProp &Prop);
	void RemovePropsAt(FIntVector Coordinates);
	void RemovePropById(uint32 PropId);
	TArray<FProp> GetPropsAt(FIntVector Coordinates);
	void AddUnit(ATBSUnit* Unit);
	TArray<ATBSUnit*>::TIterator GetUnitIterator();

	ATBSUnit* SelectUnit(FIntVector Coordinates);

	FOnActorNoLongerVisible OnActorNoLongerVisible;

private:	
	
	TMap<FIntVector, TArray<uint32>> OccupiedCoordinatesMap;
	TMap<uint32, FProp> PropMap;

	TArray<FIntVector> GetOccupiedCoordinates(FProp Prop);
	TArray<FIntVector> GetOccupiedCoordinates(ATBSUnit* Unit);

	//bool IsTileAccessBlockedByProp(FIntVector Coordinates, ETileSlot Slot);
};
