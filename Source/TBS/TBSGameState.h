// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "TBSPropManager.h"
#include "TBSUnitManager.h"
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

	UFUNCTION()
	void Click(FIntVector GameCoords);

	UFUNCTION()
	void HoverBegin(FIntVector GameCoords);

private:
	ATBSPropManager* PropManager;
	ATBSUnitManager* UnitManager;
};
