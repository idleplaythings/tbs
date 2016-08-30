// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSDefaultPawn.h"
#include "TBSHUD.h"
#include "TBSPlayerController.h"
#include "TBSGameState.h"
#include "TBSPlayerState.h"
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
	PlayerStateClass = ATBSPlayerState::StaticClass();
}

void ATBSGameMode::InitGame(const FString & MapName, const FString & Options, FString & ErrorMessage)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSGameMode::InitGame")));

	Super::InitGame(MapName, Options, ErrorMessage);	
}

void ATBSGameMode::InitGameState()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSGameMode::InitGameState")));

	Super::InitGameState();

	GetGameState<ATBSGameState>()->StartGameplay();
}

FString ATBSGameMode::InitNewPlayer(
	class APlayerController * NewPlayerController,
	const TSharedPtr <const FUniqueNetId> & UniqueId,
	const FString & Options,
	const FString & Portal
)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSGameMode::InitNewPlayer")));

	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);	
}

void ATBSGameMode::PostLogin(APlayerController* PlayerController)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSGameMode::PostLogin")));

	Super::PostLogin(PlayerController);

	//int32 TeamNumber = NumberOfPlayers;
	//NumberOfPlayers = NumberOfPlayers + 1;

	//ATBSGameState* GameState = GetGameState<ATBSGameState>();
	//Cast<ATBSPlayerController>(PlayerController)->TeamNumber = TeamNumber;

	GetGameState<ATBSGameState>()->AddPlayer(PlayerController);
}
