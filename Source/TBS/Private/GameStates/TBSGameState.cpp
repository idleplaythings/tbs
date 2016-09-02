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
	InitGrid(FIntVector(100, 100, 3));
	InitGridUI();

	if (HasAuthority())
	{
		InitFactoriesAndManagers();

		int PropsSpawned = 0;

		while (PropsSpawned < 30)
		{
			FIntVector Coordinates = FIntVector(FMath::RandRange(42, 57), FMath::RandRange(42, 57), 0);
			int32 Rotation = (float) FMath::RandRange(0, 3) * 90;

			if (Grid->SelectProp(Coordinates))
			{
				continue;
			}

			Grid->AddProp(PropFactory->CreateWall(Coordinates, FRotator(0.0, Rotation, 0.0)));
			PropsSpawned += 1;
		}
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
	int32 UnitsSpawned = 0;
	int32 XOffset = 0;

	if (PlayerNumber > 0)
	{
		XOffset = 10;
	}

	while (UnitsSpawned < 3)
	{
		FIntVector Coordinates = FIntVector(FMath::RandRange(40 + XOffset, 45 + XOffset), FMath::RandRange(45, 55), 0);

		if (Grid->SelectUnit(Coordinates))
		{
			continue;
		}

		if (Grid->SelectProp(Coordinates))
		{
			continue;
		}

		ATBSUnit* Unit = UnitFactory->CreateUnit(Coordinates, FRotator(0.0, 0.0, 0.0));
		Unit->PlayerNumber = PlayerNumber;

		Grid->AddUnit(Unit);
		UnitManager->ResetUnit(Unit);

		UnitsSpawned += 1;
	}

	Grid->ReindexUnits();
}
