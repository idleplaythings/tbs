// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSProp.h"
#include "TBSUnit.h"
//#include <string>
//#include <unordered_map>
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

	TMap<FIntVector, TArray<FProp>> PropMap;
	TMap<uint32, FIntVector> PropIndexMap;

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
	bool CanDrawLineOfFire(FIntVector Start, FIntVector End);

	TArray<FIntVector> Trace(FIntVector Start, FIntVector End);

	void InitialiseGrid(FIntVector GridDimensions);
	TArray<FIntVector> GetAccessibleNeighbours(FIntVector Coordinates);
	TArray<FIntVector> GetNeighbours(FIntVector Coordinates);
	bool IsAccessible(FIntVector Coordinates);

	void AddProp(ATBSProp* Prop);
	void AddProp(FProp Prop);
	void RemovePropsAt(FIntVector Coordinates);
	void RemovePropById(uint32 PropId);
	void AddUnit(ATBSUnit* Unit);
	TArray<ATBSUnit*>::TIterator GetUnitIterator();

	ATBSUnit* SelectUnit(FIntVector Coordinates);

	FOnActorNoLongerVisible OnActorNoLongerVisible;

	TMap<FIntVector, TArray<FProp>>::TConstIterator GetPropConstIterator();
	TMap<FIntVector, TArray<FProp>>::TIterator GetPropIterator();

private:	
	
	//bool IsTileAccessBlockedByProp(FIntVector Coordinates, ETileSlot Slot);
};
