// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSUnit.h"
#include "TBSGrid.h"
#include "TBSGridUI.h"
#include "TBSPropFactory.h"
#include "TBSPlayerController.h"
#include "Engine/ActorChannel.h"
#include "TBSGameState.h"

void ATBSGameState::StartGameplay()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSGameState::StartGameplay")));

	Grid = GetWorld()->SpawnActor<ATBSGrid>(ATBSGrid::StaticClass());
	Grid->InitialiseGrid(FIntVector(10, 20, 3));

	ATBSPropFactory* PropFactory = GetWorld()->SpawnActor<ATBSPropFactory>(ATBSPropFactory::StaticClass());

	Grid->AddProp(PropFactory->CreateWall(FIntVector(2, 8, 0), FRotator(0.0, 90.0, 0.0)));
	Grid->AddProp(PropFactory->CreateWall(FIntVector(2, 7, 0), FRotator(0.0, 0.0, 0.0)));
	Grid->AddProp(PropFactory->CreateWall(FIntVector(3, 7, 0), FRotator(0.0, 0.0, 0.0)));
	Grid->AddProp(PropFactory->CreateWall(FIntVector(4, 7, 0), FRotator(0.0, 0.0, 0.0)));

	if (HasAuthority())
	{
		Grid->OnActorNoLongerVisible.AddDynamic(this, &ATBSGameState::ForceCloseActorChannel);
	}

	UnitFactory = GetWorld()->SpawnActor<ATBSUnitFactory>(ATBSUnitFactory::StaticClass());

	GridUI = GetWorld()->SpawnActor<ATBSGridUI>(ATBSGridUI::StaticClass());
	GridUI->RenderGrid(Grid);

	PropManager = GetWorld()->SpawnActor<ATBSPropManager>(ATBSPropManager::StaticClass());
	PropManager->Initialise(Grid, GridUI);
	PropManager->ResetProps();

	UnitManager = GetWorld()->SpawnActor<ATBSUnitManager>(ATBSUnitManager::StaticClass());
	UnitManager->Initialise(Grid, GridUI);
	UnitManager->ResetUnits();
}

/**
 * 
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

void ATBSGameState::AddPlayer(APlayerController* PlayerController)
{
	ATBSUnit* Unit;
	FIntVector StartCoordinates = NumberOfPlayers == 0 ? FIntVector(3, 3, 0) : FIntVector(3, 11, 0);

	PlayerControllers.Add(NumberOfPlayers, PlayerController);

	Unit = UnitFactory->CreateUnit(StartCoordinates, FRotator(0.0, 0.0, 0.0));

	Cast<ATBSPlayerController>(PlayerController)->PlayerNumber = NumberOfPlayers;
	
	Unit->PlayerNumber = NumberOfPlayers;
	Grid->AddUnit(Unit);
	UnitManager->ResetUnit(Unit);

	NumberOfPlayers++;
}
