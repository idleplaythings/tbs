// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSUnit.h"
#include "TBSGrid.h"
#include "TBSGridUI.h"
#include "TBSPropFactory.h"
#include "TBSGameState.h"

void ATBSGameState::StartGameplay()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSGameState::StartGameplay")));

	Grid = GetWorld()->SpawnActor<ATBSGrid>(ATBSGrid::StaticClass());
	Grid->InitialiseGrid(FIntVector(10, 20, 3));

	ATBSPropFactory* PropFactory = GetWorld()->SpawnActor<ATBSPropFactory>(ATBSPropFactory::StaticClass());
	FProp Wall1 = PropFactory->CreateWall(FIntVector(2, 7, 0), ETileSlot::W, FRotator(0.0, 90.0, 0.0));
	FProp Wall2 = PropFactory->CreateWall(FIntVector(2, 7, 0), ETileSlot::N, FRotator(0.0, 0.0, 0.0));
	FProp Wall3 = PropFactory->CreateWall(FIntVector(3, 7, 0), ETileSlot::N, FRotator(0.0, 0.0, 0.0));
	FProp Wall4 = PropFactory->CreateWall(FIntVector(4, 7, 0), ETileSlot::N, FRotator(0.0, 0.0, 0.0));

	Grid->AddProp(Wall1);
	Grid->AddProp(Wall2);
	Grid->AddProp(Wall3);
	Grid->AddProp(Wall4);

	UnitFactory = GetWorld()->SpawnActor<ATBSUnitFactory>(ATBSUnitFactory::StaticClass());

	GridUI = GetWorld()->SpawnActor<ATBSGridUI>(ATBSGridUI::StaticClass());
	GridUI->RenderGrid(Grid);

	PropManager = GetWorld()->SpawnActor<ATBSPropManager>(ATBSPropManager::StaticClass());
	PropManager->Initialise(Grid, GridUI);
	PropManager->RenderProps();

	UnitManager = GetWorld()->SpawnActor<ATBSUnitManager>(ATBSUnitManager::StaticClass());
	UnitManager->Initialise(Grid, GridUI);
	UnitManager->ResetUnits();
}

void ATBSGameState::SpawnUnitsForPlayer(APlayerController* PlayerController)
{
	ATBSUnit* Unit = UnitFactory->CreateUnit(FIntVector(FMath::RandRange(0, 10), FMath::RandRange(0, 10), 0), FRotator(0.0, 0.0, 0.0));
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
