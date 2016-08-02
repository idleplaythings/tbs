// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
//#include "TBSPlayerController.h"
#include "TBSGrid.h"
#include "TBSGridUI.generated.h"

class ATBSPlayerController;

UCLASS(Blueprintable)
class TBS_API ATBSGridUI : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATBSGridUI();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	void RenderGrid(ATBSGrid* Grid);
	void LevelUp();
	void LevelDown();
	int32 GetCurrentLevel();

	bool bResolveGridCoordinates = true;

	void HandleMouseDown();
	FCoordinateLocations GetCoordinateLocations(FIntVector Coordinates);

private:
	int32 CurrentLevel = 0;
	int32 NumOfLevels;
	int32 GridWidth;
	int32 GridHeight;
	float GridMeshWidth;
	float GridMeshHeight;
	float TileSize= 100;
	float FloorHeight = 200;
	
	ATBSGrid* Grid;
	ATBSPlayerController* PlayerController;
	UStaticMesh* GridMesh;
	UMaterial* GridMaterial;
	TSubclassOf<class ATBSGridCursor> GridCursorClass;
	ATBSGridCursor* GridCursor;
	USceneComponent* SceneComponent;
	TArray<UStaticMeshComponent*> GridMeshes;
	TArray<UMaterialInstanceDynamic*> GridMaterials;
	FIntVector LastGameCoordinates = FIntVector(-999, -999, -999);

	void InitialisePlayerController();
	//void EnableAndHandleInput();
	void InitialiseParametersFromGrid();
	void CreateGridMaterialInstances();
	UMaterialInstanceDynamic* CreateMaterialInstance();	
	void CreateGridMeshComponents();
	UStaticMeshComponent* CreateMeshComponent();
	void SpawnCursor();
	void UpdateLevelVisibilities();
	void UpdateCursorLocation(const FVector Location);
	void ShowCursor();
	void HideCursor();
	//void TestThing();
	//void MouseDown();
};

