// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSDefaultPawn.h"
#include "TBSHUD.h"
#include "TBSPlayerController.h"
#include "TBSGameState.h"
#include "TBSGridUI.h"
#include "TBSGrid.h"
#include "TBSProp_Wall01.h"
#include "TBSPropManager.h"
#include "TBSGameMode.h"

ATBSGameMode::ATBSGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultPawnClass = ATBSDefaultPawn::StaticClass();
	HUDClass = ATBSHUD::StaticClass();
	PlayerControllerClass = ATBSPlayerController::StaticClass();
	GameStateClass = ATBSGameState::StaticClass();
}

void ATBSGameMode::InitGame(
	const FString & MapName,
	const FString & Options,
	FString & ErrorMessage
)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("InitGame")));
}

void ATBSGameMode::InitGameState()
{
	Super::InitGameState();

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
	Wall30.PropClass = ATBSProp_Wall01::StaticClass();
	Wall30.Coordinates = FIntVector(4, 7, 0);
	Wall30.Slot = ETileSlot::N;

	Grid->AddProp(&Wall10);
	Grid->AddProp(&Wall11);
	Grid->AddProp(&Wall20);
	Grid->AddProp(&Wall30);
	Grid->OnGameTileSelectBegin.AddDynamic(this, &ATBSGameMode::Click);
	Grid->OnGameTileHoverBegin.AddDynamic(this, &ATBSGameMode::HoverBegin);

	ATBSGridUI* GridUI = GetWorld()->SpawnActor<ATBSGridUI>(ATBSGridUI::StaticClass());
	GridUI->RenderGrid(Grid);

	ATBSPropManager* PropManager = GetWorld()->SpawnActor<ATBSPropManager>(ATBSPropManager::StaticClass());
	PropManager->Initialise(Grid, GridUI);
	PropManager->RenderProps();

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("InitGameState")));
}

void ATBSGameMode::Click(FIntVector GameCoords)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Click (%i, %i, %i)"), GameCoords.X, GameCoords.Y, GameCoords.Z));
}

void ATBSGameMode::HoverBegin(FIntVector GameCoords)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Hover (%i, %i, %i)"), GameCoords.X, GameCoords.Y, GameCoords.Z));
}

FString ATBSGameMode::InitNewPlayer(
	class APlayerController * NewPlayerController,
	const TSharedPtr < const FUniqueNetId > & UniqueId,
	const FString & Options,
	const FString & Portal
)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("InitNewPlayer")));

	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}
