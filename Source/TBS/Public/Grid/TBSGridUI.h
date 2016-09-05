// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
//#include "TBSPlayerController.h"
#include "TBSGrid.h"
#include "TBSGridUI.generated.h"

class ATBSPlayerController;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameTileHoverBegin, FIntVector, GameCoordinates);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameTileHoverEnd, FIntVector, GameCoordinates);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameTileMouseLeft, FIntVector, GameCoordinates);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameTileMouseRight, FIntVector, GameCoordinates);

UCLASS(Blueprintable)
class TBS_API ATBSGridUI : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATBSGridUI();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RenderGrid(ATBSGrid* Grid);
	void HandleGridHitResult(FHitResult HitResult);
	FCoordinateLocations GetCoordinateLocations(FIntVector Coordinates);
	void LevelUp();
	void LevelDown();	

	bool CoordinatesChanged = false;
	bool CursorOnGrid = false;
	FIntVector CurrentCoordinates = FIntVector(-999, -999, -999);
	FIntVector PreviousCoordinates = FIntVector(-999, -999, -999);

	void SelectLocation(FIntVector Dimensions, FIntVector Coordinates);
	void ClearSelection();

	void SetCursorDimensions(FIntVector InCursorDimensions);
	void SetCursorCoordinates(FIntVector Coordinates);

	void ForceCursor(FIntVector Dimensions, FIntVector Coordinates);
	void ReleaseCursor();
	void ResetCursorDimensions();

private:
	void UpdateLevelVisibilities();
	void SpawnCursor();	
	void ShowCursor();
	void HideCursor();

	FIntVector NullCoordinates = FIntVector(-999, -999, -999);
	int32 CurrentLevel = 0;
	float GridMeshWidth;
	float GridMeshHeight;
	float TileSize = 50;
	float FloorHeight = 200;

	
	void InitialiseParametersFromGrid();
	void CreateGridMaterialInstances();
	void CreateGridMeshComponents();
	UMaterialInstanceDynamic* CreateMaterialInstance();
	UStaticMeshComponent* CreateMeshComponent();

	ATBSGrid* Grid;
	UStaticMesh* GridMesh;
	UMaterial* GridMaterial;
	TSubclassOf<class ATBSGridCursor> GridCursorClass;
	ATBSGridCursor* GridCursor;
	USceneComponent* SceneComponent;
	TArray<UStaticMeshComponent*> GridMeshes;
	TArray<UMaterialInstanceDynamic*> GridMaterials;

	ATBSGridCursor* SelectionCursor;
	TArray<FIntVector> SelectedCoordinates;

	FIntVector DefaultCursorDimensions = FIntVector(1, 1, 1);
	FIntVector CursorDimensions = DefaultCursorDimensions;
	bool CursorForced = true;
};

