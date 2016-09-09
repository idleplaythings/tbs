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
	InitGrid(FIntVector(1000, 1000, 18));
	InitGridUI();

	if (HasAuthority())
	{
		InitFactoriesAndManagers();

		//int PropsSpawned = 0;

		//while (PropsSpawned < 20)
		//{
		//	FIntVector Coordinates = FIntVector(FMath::RandRange(10, 90)*10, FMath::RandRange(10, 90)*10, 0);
		//	int32 Rotation = (float) FMath::RandRange(0, 3) * 90;

		//	if (Grid->SelectProp(Coordinates))
		//	{
		//		continue;
		//	}

		//	//Grid->AddProp(PropFactory->CreateWall(Coordinates, FRotator(0.0, Rotation, 0.0)));
		//	Grid->AddProp(PropFactory->CreateBlock(Coordinates, FIntVector(1, 1, 6), FRotator(0.0, Rotation, 0.0)));

		//	PropsSpawned += 1;
		//}
		//PropManager->ResetProps();

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
	Controller->OnClientReady.AddDynamic(this, &ATBSGameState::ClientReady);

	PlayerControllers.Add(PlayerNumber, Controller);	
}

void ATBSGameState::ClientReady()
{
	NumberOfPlayersReady += 1;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Clients ready %i/%i"), NumberOfPlayersReady, NumberOfPlayersExpected));

	if (NumberOfPlayersReady == NumberOfPlayersExpected)
	{
		AllClientsReady();
	}
}

void ATBSGameState::AllClientsReady()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("All clients ready")));

	for (int32 i = 0; i < PropsToSend; i++)
	{
		FIntVector Coordinates = FIntVector(FMath::RandRange(10, 90) * 10, FMath::RandRange(10, 90) * 10, 0);
		int32 Rotation = (float)FMath::RandRange(0, 3) * 90;
		PropFactory->CreateBlock(Coordinates, FIntVector(1, 1, 6), FRotator(0.0, Rotation, 0.0));
	}

	GetWorldTimerManager().SetTimer(SendTimer, this, &ATBSGameState::SendProps, Delay, true);
}

void ATBSGameState::SendProps()
{
	int32 PropsLeft = PropsToSend - PropsSent;

	if (PropsLeft <= 0)
	{
		GetWorldTimerManager().ClearTimer(SendTimer);
		return;
	}

	int32 SendSize = PropsLeft >= BatchSize ? BatchSize : PropsLeft;

	TArray<FProp> PropArray;

	for (int32 i = 0; i < SendSize; i++)
	{
		FProp Prop;
		Prop.Id = 1;
		Prop.Coordinates = FIntVector(FMath::RandRange(0, Grid->GridDimensions.X), FMath::RandRange(0, Grid->GridDimensions.Y), 0);

		PropArray.Add(Prop);
	}

	//Grid->UpdateProps(PropArray);

	for (auto& It : PlayerControllers)
	{
		(*It.Value).Client_CreateProps(PropArray);
	}

	PropsSent += SendSize;
}

void ATBSGameState::SpawnUnits(int32 PlayerNumber)
{
	int32 UnitsSpawned = 0;
	int32 XOffset = 0;

	if (PlayerNumber > 0)
	{
		XOffset = 20;
	}

	while (UnitsSpawned < 3)
	{
		FIntVector Coordinates;

		if (UnitsSpawned == 0)
		{
			Coordinates = FIntVector(FMath::RandRange(30 + XOffset, 45 + XOffset) * 10 + 5, FMath::RandRange(30, 60) * 10 + 5, 0);
		}
		else
		{
			Coordinates = FIntVector(FMath::RandRange(30 + XOffset, 45 + XOffset) * 10, FMath::RandRange(30, 60) * 10, 0);
		}

		if (Grid->SelectUnit(Coordinates))
		{
			continue;
		}

		if (Grid->SelectProp(Coordinates))
		{
			continue;
		}

		ATBSUnit* Unit;

		if (UnitsSpawned == 0)
		{
			Unit = UnitFactory->CreateUnit(Coordinates, FRotator(0.0, 0.0, 0.0));			
		}
		else if (UnitsSpawned == 1)
		{
			Unit = UnitFactory->CreateLargeUnit(Coordinates, FRotator(0.0, 0.0, 0.0));
		}
		else
		{
			Unit = UnitFactory->CreateSmallUnit(Coordinates, FRotator(0.0, 0.0, 0.0));
		}
		
		Unit->PlayerNumber = PlayerNumber;

		Grid->AddUnit(Unit);
		UnitManager->ResetUnit(Unit);

		UnitsSpawned += 1;
	}

	Grid->ReindexUnits();
}
