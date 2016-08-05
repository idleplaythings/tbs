// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "TBSPropManager.h"
#include "TBSUnitManager.h"
#include "TBSGridPathFinder.h"
#include "TBSGameState.generated.h"

/**
 * 
 */
UCLASS()
class TBS_API ATBSGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ATBSGameState();

	void StartGameplay();

	UFUNCTION()
	void MouseLeft(FIntVector GameCoords);

	UFUNCTION()
	void MouseRight(FIntVector GameCoords);

	UFUNCTION()
	void HoverBegin(FIntVector GameCoords);

	UFUNCTION()
	void HoverEnd(FIntVector GameCoords);

private:
	ATBSGrid* Grid;
	ATBSGridUI* GridUI;
	ATBSPropManager* PropManager;
	ATBSUnitManager* UnitManager;
	ATBSGridPathFinder* GridPathFinder;

	FUnit SelectedUnit;
	bool UnitSelected = false;

	void RenderPath(TArray<FIntVector> Path);
	void ClearPath();
	TArray<AActor*> RenderedPath;	
	UClass* PathComponentClass;
};
