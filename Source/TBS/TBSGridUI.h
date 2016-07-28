// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSPlayerController.h"
#include "TBSGridUI.generated.h"

UCLASS(Blueprintable)
class TBS_API ATBSGridUI : public AActor
{
	GENERATED_BODY()

private:
	int CurrentLevel = 0;
	int NumOfLevels;
	int GridWidth;
	int GridHeight;
	float GridMeshWidth;
	float GridMeshHeight;
	float TileSize= 100;
	float FloorHeight = 200;
	
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
	void InitialiseParametersFromGameMode();
	void CreateGridMaterialInstances();
	UMaterialInstanceDynamic* CreateMaterialInstance();	
	void CreateGridMeshComponents();
	UStaticMeshComponent* CreateMeshComponent();
	void SpawnCursor();
	void UpdateLevelVisibilities();
	void UpdateCursorLocation(const FVector Location);

public:
	// Sets default values for this actor's properties
	ATBSGridUI();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void LevelUp();
	void LevelDown();
	int GetCurrentLevel();

	bool bResolveGridCoordinates = true;
};

