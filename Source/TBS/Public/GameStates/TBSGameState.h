// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "TBSUnitManager.h"
#include "TBSUnitFactory.h"
#include "TBSPropManager.h"
#include "TBSPropFactory.h"
#include "TBSGridPathFinder.h"
#include "TBSGridPathRenderer.h"
#include "TBSPlayerController.h"
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

	UPROPERTY(Replicated)
	int32 NumberOfPlayers = 0;
	int32 NumberOfPlayersReady = 0;
	int32 NumberOfPlayersExpected = 2;

	UFUNCTION()
	void ForceCloseActorChannel(int32 TeamNumber, AActor* Actor);

	// TODO: Player controller still depends on this
	ATBSUnitManager* UnitManager;

	UFUNCTION()
	void ClientReady();

private:
	void InitGrid(FIntVector Dimensions);
	void InitGridUI();
	void InitFactoriesAndManagers();
	void InitPlayerController(int32 PlayerNumber, APlayerController* PlayerController);
	void SpawnUnits(int32 PlayerNumber);

	void AllClientsReady();

	ATBSPropManager* PropManager;
	ATBSPropFactory* PropFactory;
	ATBSUnitFactory* UnitFactory;
	ATBSGrid* Grid;
	ATBSGridUI* GridUI;
	TMap<int32, ATBSPlayerController*> PlayerControllers;


	FTimerHandle SendTimer;
	int32 PropsToSend = 10000;
	int32 PropsSent = 0;
	int32 BatchSize = 2048;
	float Delay = 0.5;

	void SendProps();
};
