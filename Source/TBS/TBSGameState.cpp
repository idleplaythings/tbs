// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSGrid.h"
#include "TBSGridUI.h"
#include "TBSUnitFactory.h"
#include "TBSProp_Wall01.h"
#include "TBSGameState.h"

void ATBSGameState::StartGameplay()
{
	ATBSGrid* Grid = GetWorld()->SpawnActor<ATBSGrid>(ATBSGrid::StaticClass());
	Grid->InitialiseGrid(10, 20, 3);

	FProp Wall10, Wall11, Wall20, Wall30;

	Wall10.PropClass = ATBSProp_Wall01::StaticClass();
	Wall10.Coordinates = FIntVector(2, 7, 0);
	Wall10.Slot = ETileSlot::W;
	Wall10.Rotation = FRotator(0.0, 90.0, 0.0);

	Wall11.PropClass = ATBSProp_Wall01::StaticClass();
	Wall11.Coordinates = FIntVector(2, 7, 0);
	Wall11.Slot = ETileSlot::N;
	Wall11.Rotation = FRotator(0.0, 0.0, 0.0);

	Wall20.PropClass = ATBSProp_Wall01::StaticClass();
	Wall20.Coordinates = FIntVector(3, 7, 0);
	Wall20.Slot = ETileSlot::N;
	Wall20.Rotation = FRotator(0.0, 0.0, 0.0);

	Wall30.PropClass = ATBSProp_Wall01::StaticClass();
	Wall30.Coordinates = FIntVector(4, 7, 0);
	Wall30.Slot = ETileSlot::N;
	Wall30.Rotation = FRotator(0.0, 0.0, 0.0);

	Grid->AddProp(&Wall10);
	Grid->AddProp(&Wall11);
	Grid->AddProp(&Wall20);
	Grid->AddProp(&Wall30);

	ATBSUnitFactory* UnitFactory = GetWorld()->SpawnActor<ATBSUnitFactory>(ATBSUnitFactory::StaticClass());
	FUnit Unit10 = UnitFactory->CreateUnit(FIntVector(1, 1, 0), FRotator(0.0, 0.0, 0.0));
	Grid->AddUnit(&Unit10);

	ATBSGridUI* GridUI = GetWorld()->SpawnActor<ATBSGridUI>(ATBSGridUI::StaticClass());
	GridUI->OnGameTileSelectBegin.AddDynamic(this, &ATBSGameState::Click);
	GridUI->OnGameTileHoverBegin.AddDynamic(this, &ATBSGameState::HoverBegin);
	GridUI->RenderGrid(Grid);

	PropManager = GetWorld()->SpawnActor<ATBSPropManager>(ATBSPropManager::StaticClass());
	PropManager->Initialise(Grid, GridUI);
	PropManager->RenderProps();

	UnitManager = GetWorld()->SpawnActor<ATBSUnitManager>(ATBSUnitManager::StaticClass());
	UnitManager->Initialise(Grid, GridUI);
	UnitManager->RenderUnits();

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("InitGameState")));
}

void ATBSGameState::Click(FIntVector GameCoords)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Click (%i, %i, %i)"), GameCoords.X, GameCoords.Y, GameCoords.Z));
}

void ATBSGameState::HoverBegin(FIntVector GameCoords)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Hover (%i, %i, %i)"), GameCoords.X, GameCoords.Y, GameCoords.Z));
}
