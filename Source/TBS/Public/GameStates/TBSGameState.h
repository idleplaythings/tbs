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
#include "TBSTCPServer.h"
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
	~ATBSGameState();

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	void StartGameplay();
	void AddPlayer(APlayerController* PlayerController);

	UPROPERTY(Replicated)
	int32 NumberOfPlayers = 0;
	int32 NumberOfPlayersReady = 0;
	int32 NumberOfPlayersExpected = 1;

	UFUNCTION()
	void ForceCloseActorChannel(int32 TeamNumber, AActor* Actor);
	
	// TODO: Player controller still depends on this
	ATBSUnitManager* UnitManager;

	UFUNCTION()
	void ClientReady();

	TBSTCPServer* TCPServer;

	TQueue<FString> NetworkMessageQueue;

	void SpawnNewProps(FIntVector Coordinates);
	void Bomb(FIntVector Coordinates);

private:
	void InitGrid(FIntVector Dimensions);
	void InitGridUI();
	void InitFactoriesAndManagers();
	void InitPlayerController(int32 PlayerNumber, APlayerController* PlayerController);
	void SpawnUnits(int32 PlayerNumber);

	void AllClientsReady();
	FIntVector FindFreeCoordinates();

	ATBSPropManager* PropManager;
	ATBSPropFactory* PropFactory;
	ATBSUnitFactory* UnitFactory;
	ATBSGrid* Grid;
	ATBSGridUI* GridUI;
	TMap<int32, ATBSPlayerController*> PlayerControllers;


	int32 PropsToGenerate = 166666 * 6; // Miljoona propsia :o
	//int32 PropsToGenerate = 30000 * 6;
	uint8_t* PropsData = nullptr;
	uint32 PropsDataLength;
	UClass* ExplosionClass;
};
