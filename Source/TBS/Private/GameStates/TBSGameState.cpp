// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSUnit.h"
#include "TBSGrid.h"
#include "TBSGridUI.h"
#include "UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "TBSGameState.h"

void ATBSGameState::StartGameplay()
{
	InitGrid(FIntVector(10, 20, 3));
	InitGridUI();

	if (HasAuthority())
	{
		InitFactoriesAndManagers();

		// Create dummy map
		Grid->AddProp(PropFactory->CreateWall(FIntVector(2, 8, 0), FRotator(0.0, 90.0, 0.0)));
		Grid->AddProp(PropFactory->CreateWall(FIntVector(2, 7, 0), FRotator(0.0, 0.0, 0.0)));
		Grid->AddProp(PropFactory->CreateWall(FIntVector(3, 7, 0), FRotator(0.0, 0.0, 0.0)));
		Grid->AddProp(PropFactory->CreateWall(FIntVector(4, 7, 0), FRotator(0.0, 0.0, 0.0)));		
		PropManager->ResetProps();

		Grid->OnActorNoLongerVisible.AddDynamic(this, &ATBSGameState::ForceCloseActorChannel);
	}
}

void ATBSGameState::AddPlayer(APlayerController* PlayerController)
{
	int32 PlayerNumber = NumberOfPlayers;

	InitPlayerController(PlayerNumber, PlayerController);
	SpawnUnits(PlayerNumber);

	NumberOfPlayers++;
}

/**
 * Force close a replication channel associated with an actor.

 * By default UE4 will keep the replication channel open for an actor for five seconds after the actor is
 * no longer relevant, which is something we specifically don't want as that might reveal hidden
 * information about unit that has moved out of enemy LOF.
 */
void ATBSGameState::ForceCloseActorChannel(int32 PlayerNumber, AActor* Actor)
{
	if (HasAuthority())
	{
		UNetConnection* Connection = PlayerControllers[PlayerNumber]->GetNetConnection();
		UActorChannel* Channel = Connection->ActorChannels.FindRef(Actor);

		if (Channel)
		{
			Channel->Close();
		}		
	}
}

void ATBSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATBSGameState, NumberOfPlayers);
}

void ATBSGameState::InitGrid(FIntVector Dimensions)
{
	Grid = GetWorld()->SpawnActor<ATBSGrid>(ATBSGrid::StaticClass());
	Grid->InitialiseGrid(Dimensions);
}

void ATBSGameState::InitGridUI()
{
	if (Grid)
	{
		GridUI = GetWorld()->SpawnActor<ATBSGridUI>(ATBSGridUI::StaticClass());
		GridUI->RenderGrid(Grid);
	}
}

void ATBSGameState::InitFactoriesAndManagers()
{
	PropFactory = GetWorld()->SpawnActor<ATBSPropFactory>(ATBSPropFactory::StaticClass());
	UnitFactory = GetWorld()->SpawnActor<ATBSUnitFactory>(ATBSUnitFactory::StaticClass());

	if (Grid && GridUI)
	{
		PropManager = GetWorld()->SpawnActor<ATBSPropManager>(ATBSPropManager::StaticClass());
		PropManager->Initialise(Grid, GridUI);

		UnitManager = GetWorld()->SpawnActor<ATBSUnitManager>(ATBSUnitManager::StaticClass());
		UnitManager->Initialise(Grid, GridUI);
	}
}

void ATBSGameState::InitPlayerController(int32 PlayerNumber, APlayerController* PlayerController)
{
	ATBSPlayerController* Controller = Cast<ATBSPlayerController>(PlayerController);
	Controller->PlayerNumber = PlayerNumber;
	PlayerControllers.Add(PlayerNumber, Controller);
}

void ATBSGameState::SpawnUnits(int32 PlayerNumber)
{
	FIntVector StartCoordinates = PlayerNumber == 0 ? FIntVector(3, 3, 0) : FIntVector(3, 11, 0);
	ATBSUnit* Unit = UnitFactory->CreateUnit(StartCoordinates, FRotator(0.0, 0.0, 0.0));
	Unit->PlayerNumber = PlayerNumber;

	Grid->AddUnit(Unit);
	UnitManager->ResetUnit(Unit);
}
