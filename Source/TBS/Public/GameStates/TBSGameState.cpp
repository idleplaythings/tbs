// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSUnit.h"
#include "TBSGrid.h"
#include "TBSGridUI.h"
#include "TBSPropFactory.h"
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

void ATBSGameState::ForceCloseActorChannel(int32 TeamNumber, AActor* Actor)
{
	if (HasAuthority())
	{
		UNetConnection* Connection = PlayerControllers[TeamNumber]->GetNetConnection();
		UActorChannel* Channel = Connection->ActorChannels.FindRef(Actor);

		if (Channel)
		{
			Channel->Close();
		}		
	}
}

void ATBSGameState::SpawnUnitsForPlayer(APlayerController* PlayerController, int32 TeamNumber)
{
	ATBSUnit* Unit;

	PlayerControllers.Add(TeamNumber, PlayerController);

	if (TeamNumber == 0)
	{
		Unit = UnitFactory->CreateUnit(FIntVector(3, 3, 0), FRotator(0.0, 0.0, 0.0));
	}
	else
	{
		Unit = UnitFactory->CreateUnit(FIntVector(3, 11, 0), FRotator(0.0, 0.0, 0.0));
	}

	/*ATBSUnit* Unit = UnitFactory->CreateUnit(FIntVector(FMath::RandRange(0, 9), FMath::RandRange(0, 9), 0), FRotator(0.0, 0.0, 0.0));*/
	Unit->TeamNumber = TeamNumber;
	Grid->AddUnit(Unit);
	UnitManager->ResetUnit(Unit);
	//Grid->AddUnit(UnitFactory->CreateUnit(FIntVector(7, 7, 0), FRotator(0.0, 0.0, 0.0)));
}

void ATBSGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSGameState::PostInitializeComponents")));
}

//void ATBSGameState::InitPlayer()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSGameState::InitPlayer")));
//
//	//GridUI = GetWorld()->SpawnActor<ATBSGridUI>(ATBSGridUI::StaticClass());
//	//GridUI->OnGameTileMouseLeft.AddDynamic(this, &ATBSGameState::MouseLeft);
//	//GridUI->OnGameTileMouseRight.AddDynamic(this, &ATBSGameState::MouseRight);
//	//GridUI->OnGameTileHoverBegin.AddDynamic(this, &ATBSGameState::HoverBegin);
//	//GridUI->OnGameTileHoverEnd.AddDynamic(this, &ATBSGameState::HoverEnd);
//	//GridUI->RenderGrid(Grid);
//
//	//PropManager = GetWorld()->SpawnActor<ATBSPropManager>(ATBSPropManager::StaticClass());
//	//PropManager->Initialise(Grid, GridUI);
//	//PropManager->RenderProps();
//
//	//UnitManager = GetWorld()->SpawnActor<ATBSUnitManager>(ATBSUnitManager::StaticClass());
//	//UnitManager->Initialise(Grid, GridUI);
//	//UnitManager->RenderUnits();
//
//	//GridPathFinder = GetWorld()->SpawnActor<ATBSGridPathFinder>(ATBSGridPathFinder::StaticClass());
//	//GridPathFinder->Initialise(Grid);
//
//	//GridPathRenderer = GetWorld()->SpawnActor<ATBSGridPathRenderer>(ATBSGridPathRenderer::StaticClass());
//	//GridPathRenderer->Initialise(Grid, GridUI);
//}
//
//void ATBSGameState::MouseRight(FIntVector GameCoords)
//{
//	if (UnitSelected)
//	{		
//		UnitSelected = false;
//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Unit deselected...")));
//	}
//
//	PathSelected = false;
//	GridPathRenderer->ClearPath();
//}
//
//void ATBSGameState::MouseLeft(FIntVector GameCoords)
//{	
//	FUnit* Unit = Grid->SelectUnit(GameCoords);
//
//	if (Unit != nullptr)
//	{
//		if (!UnitSelected || SelectedUnit->Guid != Unit->Guid)
//		{
//			SelectedUnit = Unit;
//			UnitSelected = true;
//			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Unit selected!")));
//		}
//	}
//	else
//	{
//		if (UnitSelected && PathSelected)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Adding some movement!")));
//
//			TArray<FMovement> Movements;
//
//			for (auto& Component : CurrentPath)
//			{
//				FCoordinateLocations Locations = GridUI->GetCoordinateLocations(Component);
//				FMovement Movement;
//				Movement.TargetWorldCoordinates = Locations.Center;
//				Movement.TargetGameCoordinates = Component;
//
//				UnitManager->MoveUnit(SelectedUnit, Movement);
//				UnitSelected = false;
//				GridPathRenderer->ClearPath();
//			}			
//
//			PathSelected = false;
//		}
//	}
//}
//
//void ATBSGameState::HoverBegin(FIntVector GameCoords)
//{
//	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Hover (%i, %i, %i)"), GameCoords.X, GameCoords.Y, GameCoords.Z));
//
//	if (UnitSelected)
//	{
//		PathSelected = true;
//		CurrentPath = GridPathFinder->FindPath(SelectedUnit->Coordinates, GameCoords);
//		GridPathRenderer->RenderPath(CurrentPath);
//	}
//}
//
//void ATBSGameState::HoverEnd(FIntVector GameCoords)
//{
//	GridPathRenderer->ClearPath();
//}
