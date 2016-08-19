// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"

#include "TBSPropManager.h"
#include "TBSUnitManager.h"
#include "TBSGridPathFinder.h"
#include "TBSGridPathRenderer.h"

#include "TBSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TBS_API ATBSPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	void ClientInitialize(AController* Controller);	

	//void StartGameplay();
	//void InitPlayer();
	void BeginPlay() override;
	void PostInitializeComponents() override;
	void Initialise(ATBSGrid* Grid, ATBSGridUI* GridUI);

	UFUNCTION()
	void MouseLeft(FIntVector GameCoords);

	UFUNCTION()
	void MouseRight(FIntVector GameCoords);

	UFUNCTION()
	void HoverBegin(FIntVector GameCoords);

	UFUNCTION()
	void HoverEnd(FIntVector GameCoords);

private:
	ATBSPlayerController* PlayerController;
	ATBSGrid* Grid;
	ATBSGridUI* GridUI;
	ATBSPropManager* PropManager;
	ATBSUnitManager* UnitManager;
	ATBSGridPathFinder* GridPathFinder;
	ATBSGridPathRenderer* GridPathRenderer;

	ATBSUnit* SelectedUnit;
	bool UnitSelected = false;

	bool PathSelected = false;
	TArray<FIntVector> CurrentPath;
};
