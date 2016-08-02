// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSProp.h"
#include "TBSGrid.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameTileHoverBegin, FIntVector, GameCoordinates);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameTileHoverEnd, FIntVector, GameCoordinates);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameTileSelectBegin, FIntVector, GameCoordinates);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameTileSelectEnd, FIntVector, GameCoordinates);

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
	void AddProp(FProp* Prop);
	void NotifyCursorLocationChange(FIntVector OldCoordinates, FIntVector NewCoordinates);
	void NotifyClick();
	void NotifyCursorOffGrid();

	FIntVector NullVector = FIntVector(-999, -999, -999);
	FIntVector HoverCoordinates = NullVector;
	FIntVector SelectCoordinates = NullVector;

	FOnGameTileHoverBegin OnGameTileHoverBegin;
	FOnGameTileHoverEnd OnGameTileHoverEnd;
	FOnGameTileSelectBegin OnGameTileSelectBegin;
	FOnGameTileSelectEnd OnGameTileSelectEnd;
	TMap<FString, TArray<FProp*>> Props;

private:
	
};
