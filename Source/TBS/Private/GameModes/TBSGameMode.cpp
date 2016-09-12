// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSDefaultPawn.h"
#include "TBSHUD.h"
#include "TBSPlayerController.h"
#include "TBSGameState.h"
#include "TBSPlayerState.h"
#include "TBSGameMode.h"

ATBSGameMode::ATBSGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultPawnClass = ATBSDefaultPawn::StaticClass();
	HUDClass = ATBSHUD::StaticClass();
	PlayerControllerClass = ATBSPlayerController::StaticClass();
	GameStateClass = ATBSGameState::StaticClass();
	PlayerStateClass = ATBSPlayerState::StaticClass();
}

void ATBSGameMode::InitGameState()
{
	Super::InitGameState();

	GetGameState<ATBSGameState>()->StartGameplay();
}

void ATBSGameMode::PostLogin(APlayerController* PlayerController)
{
	Super::PostLogin(PlayerController);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Client joined, address %s"), *PlayerController->GetPlayerNetworkAddress()));

	GetGameState<ATBSGameState>()->AddPlayer(PlayerController);
}
