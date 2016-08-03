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

	ATBSGameState* GameState = GetGameState<ATBSGameState>();
	GameState->StartGameplay();
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
