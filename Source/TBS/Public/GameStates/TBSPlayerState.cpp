// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"

#include "TBSUnit.h"
#include "TBSGameMode.h"
#include "TBSGameState.h"
#include "TBSPlayerController.h"
#include "TBSGrid.h"
#include "TBSGridUI.h"
#include "TBSUnitFactory.h"
#include "TBSPlayerController.h"
#include "TBSPropFactory.h"

#include "TBSPlayerState.h"

void ATBSPlayerState::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSPlayerState::BeginPlay")));

	//if (Grid)
	//{
	//	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Creating grid ui")));
	//	GridUI = GetWorld()->SpawnActor<ATBSGridUI>(ATBSGridUI::StaticClass());
	//	GridUI->OnGameTileMouseLeft.AddDynamic(this, &ATBSPlayerState::MouseLeft);
	//  GridUI->OnGameTileMouseRight.AddDynamic(this, &ATBSPlayerState::MouseRight);
	//	GridUI->OnGameTileHoverBegin.AddDynamic(this, &ATBSPlayerState::HoverBegin);
	//	GridUI->OnGameTileHoverEnd.AddDynamic(this, &ATBSPlayerState::HoverEnd);
	//	GridUI->RenderGrid(Grid);
	//}
}

void ATBSPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSGameState::PostInitializeComponents")));
}

void ATBSPlayerState::Initialise(ATBSGrid* InGrid, ATBSGridUI* InGridUI)
{
	Grid = InGrid;
	GridUI = InGridUI;

	GridPathFinder = GetWorld()->SpawnActor<ATBSGridPathFinder>(ATBSGridPathFinder::StaticClass());
	GridPathFinder->Initialise(Grid);

	GridPathRenderer = GetWorld()->SpawnActor<ATBSGridPathRenderer>(ATBSGridPathRenderer::StaticClass());
	GridPathRenderer->Initialise(Grid, GridUI);
}

void ATBSPlayerState::ClientInitialize(AController* Controller)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSPlayerState::ClientInitialize")));

	Super::ClientInitialize(Controller);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Controller %p"), Controller));

	PlayerController = Cast<ATBSPlayerController>(Controller);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("PlayerController %p"), PlayerController));

	//Grid = GetWorld()->SpawnActor<ATBSGrid>(ATBSGrid::StaticClass());
	//Grid->InitialiseGrid(10, 20, 3);

	//ATBSPropFactory* PropFactory = GetWorld()->SpawnActor<ATBSPropFactory>(ATBSPropFactory::StaticClass());
	//FProp Wall1 = PropFactory->CreateWall(FIntVector(2, 7, 0), ETileSlot::W, FRotator(0.0, 90.0, 0.0));
	//FProp Wall2 = PropFactory->CreateWall(FIntVector(2, 7, 0), ETileSlot::N, FRotator(0.0, 0.0, 0.0));
	//FProp Wall3 = PropFactory->CreateWall(FIntVector(3, 7, 0), ETileSlot::N, FRotator(0.0, 0.0, 0.0));
	//FProp Wall4 = PropFactory->CreateWall(FIntVector(4, 7, 0), ETileSlot::N, FRotator(0.0, 0.0, 0.0));

	//Grid->AddProp(Wall1);
	//Grid->AddProp(Wall2);
	//Grid->AddProp(Wall3);
	//Grid->AddProp(Wall4);

	//ATBSUnitFactory* UnitFactory = GetWorld()->SpawnActor<ATBSUnitFactory>(ATBSUnitFactory::StaticClass());
	//Grid->AddUnit(UnitFactory->CreateUnit(FIntVector(1, 1, 0), FRotator(0.0, 0.0, 0.0)));
	//Grid->AddUnit(UnitFactory->CreateUnit(FIntVector(7, 7, 0), FRotator(0.0, 0.0, 0.0)));

	//for (TActorIterator<ATBSGrid> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Init found grid")));
	//	Grid = *ActorItr;
	//	break;
	//}

	//for (TActorIterator<ATBSGridUI> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Init found grid UI")));
	//	GridUI = *ActorItr;
	//	break;
	//}

	//if (Grid && GridUI)
	//{
	//	GridUI->RenderGrid(Grid);
	//}	

	//if (Grid)
	//{
	

	//}

	//PropManager = GetWorld()->SpawnActor<ATBSPropManager>(ATBSPropManager::StaticClass());
	//PropManager->Initialise(Grid, GridUI);
	//PropManager->RenderProps();


	/*if (Grid)
	{
		GridPathFinder = GetWorld()->SpawnActor<ATBSGridPathFinder>(ATBSGridPathFinder::StaticClass());
		GridPathFinder->Initialise(Grid);
	}

	if (Grid && GridUI)
	{
		GridPathRenderer = GetWorld()->SpawnActor<ATBSGridPathRenderer>(ATBSGridPathRenderer::StaticClass());
		GridPathRenderer->Initialise(Grid, GridUI);
	}
*/
	//Cast<ATBSPlayerController>(Controller)->Initialise();	
}


void ATBSPlayerState::MouseRight(FIntVector GameCoords)
{
	if (UnitSelected)
	{
		UnitSelected = false;

		//PlayerController->Server_UnPossess();
		
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Unit deselected...")));
	}

	PathSelected = false;
	GridPathRenderer->ClearPath();
}

void ATBSPlayerState::MouseLeft(FIntVector GameCoords)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSPlayerState::MouseLeft")));

	ATBSUnit* Unit = Grid->SelectUnit(GameCoords);

	if (Unit != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Found unit!")));

		if (!UnitSelected || SelectedUnit != Unit)
		{
			SelectedUnit = Unit;
			UnitSelected = true;
			//PlayerController->Server_Possess(Unit);
			
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Unit selected!")));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("No unit found!")));

		if (UnitSelected && PathSelected)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Adding some movement!")));

			for (auto& Component : CurrentPath)
			{
				FCoordinateLocations Locations = GridUI->GetCoordinateLocations(Component);
				FMovement Movement;
				Movement.TargetWorldCoordinates = Locations.Center;
				Movement.TargetGameCoordinates = Component;				

				SelectedUnit->AddMovementCommand();
				//UnitManager->MoveUnit(SelectedUnit, Movement);
				UnitSelected = false;
				GridPathRenderer->ClearPath();
			}

			PathSelected = false;
		}
	}
}

void ATBSPlayerState::HoverBegin(FIntVector GameCoords)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Hover (%i, %i, %i)"), GameCoords.X, GameCoords.Y, GameCoords.Z));

	if (UnitSelected)
	{
		PathSelected = true;

		if (GridPathFinder)
		{
			CurrentPath = GridPathFinder->FindPath(SelectedUnit->GameCoordinates, GameCoords);

			if (GridPathRenderer)
			{
				GridPathRenderer->RenderPath(CurrentPath);
			}
		}		
	}
}

void ATBSPlayerState::HoverEnd(FIntVector GameCoords)
{
	if (GridPathRenderer)
	{
		GridPathRenderer->ClearPath();
	}	
}
