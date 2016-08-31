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

//void ATBSPlayerState::Initialise(ATBSGrid* InGrid, ATBSGridUI* InGridUI)
//{
//	Grid = InGrid;
//	GridUI = InGridUI;
//
//	GridPathFinder = GetWorld()->SpawnActor<ATBSGridPathFinder>(ATBSGridPathFinder::StaticClass());
//	GridPathFinder->Initialise(Grid);
//
//	GridPathRenderer = GetWorld()->SpawnActor<ATBSGridPathRenderer>(ATBSGridPathRenderer::StaticClass());
//	GridPathRenderer->Initialise(Grid, GridUI);
//}

void ATBSPlayerState::ClientInitialize(AController* Controller)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSPlayerState::ClientInitialize")));

	Super::ClientInitialize(Controller);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Controller %p"), Controller));

	PlayerController = Cast<ATBSPlayerController>(Controller);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("PlayerController %p"), PlayerController));
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
	//GridPathRenderer->ClearPath();
}

void ATBSPlayerState::MouseLeft(FIntVector GameCoords)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSPlayerState::MouseLeft")));

	//ATBSUnit* Unit = Grid->SelectUnit(GameCoords);

	//if (Unit != nullptr)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Found unit!")));

	//	if (Unit->PlayerNumber == PlayerController->PlayerNumber)
	//	{
	//		if (!UnitSelected || SelectedUnit != Unit)
	//		{
	//			SelectedUnit = Unit;
	//			UnitSelected = true;
	//			//PlayerController->Server_Possess(Unit);
	//			//PlayerController->Server_HandleCommand(FCommand(SelectedUnit));

	//			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Unit selected!")));
	//		}
	//	}
	//	else
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Not your unit")));
	//	}
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("No unit found!")));

	//	if (UnitSelected && PathSelected)
	//	{
	//		TArray<FIntVector> Movements;

	//		for (auto& Component : CurrentPath)
	//		{
	//			Movements.Add(Component);
	//		}

	//		PlayerController->Server_HandleCommand(SelectedUnit, Movements);

	//		UnitSelected = false;
	//		//GridPathRenderer->ClearPath();

	//		PathSelected = false;
	//	}
	//}
}

void ATBSPlayerState::HoverBegin(FIntVector GameCoords)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Hover (%i, %i, %i)"), GameCoords.X, GameCoords.Y, GameCoords.Z));

	if (UnitSelected)
	{
		PathSelected = true;

		//if (GridPathFinder)
		//{			
		//	if (GridPathFinder->FindPath(SelectedUnit->GameCoordinates, GameCoords, CurrentPath))
		//	{
		//		if (GridPathRenderer)
		//		{
		//			GridPathRenderer->RenderPath(CurrentPath);
		//		}
		//	}
		//}		
	}
}

void ATBSPlayerState::HoverEnd(FIntVector GameCoords)
{
	//if (GridPathRenderer)
	//{
	//	GridPathRenderer->ClearPath();
	//}	
}
