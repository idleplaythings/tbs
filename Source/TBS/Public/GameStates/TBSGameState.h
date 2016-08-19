// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "TBSPropManager.h"
#include "TBSUnitManager.h"
#include "TBSUnitFactory.h"
#include "TBSGridPathFinder.h"
#include "TBSGridPathRenderer.h"
#include "TBSGameState.generated.h"

/**
 * 
 */
UCLASS()
class TBS_API ATBSGameState : public AGameState
{
	GENERATED_BODY()
	
public:

	void StartGameplay();
	void PostInitializeComponents() override;

	//UFUNCTION()
	//void MouseLeft(FIntVector GameCoords);

	//UFUNCTION()
	//void MouseRight(FIntVector GameCoords);

	//UFUNCTION()
	//void HoverBegin(FIntVector GameCoords);

	//UFUNCTION()
	//void HoverEnd(FIntVector GameCoords);

	void  SpawnUnitsForPlayer(APlayerController* PlayerController);

private:
	ATBSGrid* Grid;

	ATBSPropManager* PropManager;
	ATBSUnitManager* UnitManager;
	ATBSUnitFactory* UnitFactory;

	ATBSGridUI* GridUI;
	//ATBSPropManager* PropManager;
	//ATBSUnitManager* UnitManager;
	//ATBSGridPathFinder* GridPathFinder;
	//ATBSGridPathRenderer* GridPathRenderer;

	//FUnit* SelectedUnit;
	//bool UnitSelected = false;

	//bool PathSelected = false;
	//TArray<FIntVector> CurrentPath;
};
