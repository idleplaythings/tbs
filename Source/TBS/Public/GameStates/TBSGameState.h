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
	void AddPlayer(APlayerController* PlayerController);

	int32 NumberOfPlayers;
	ATBSPropManager* PropManager;
	ATBSUnitManager* UnitManager;
	ATBSUnitFactory* UnitFactory;
	ATBSGrid* Grid;
	ATBSGridUI* GridUI;

	UFUNCTION()
	void ForceCloseActorChannel(int32 TeamNumber, AActor* Actor);

private:
	TMap<int32, APlayerController*> PlayerControllers;
};
