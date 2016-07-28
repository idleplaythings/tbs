// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSDefaultPawn.h"
#include "TBSHUD.h"
#include "TBSPlayerController.h"
#include "TBSGameState.h"
#include "TBSGridUI.h"
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

	ATBSGridUI* GridUI = GetWorld()->SpawnActor<ATBSGridUI>(ATBSGridUI::StaticClass());
	//GridUI->SetGrid()

	//int GridWidth = 20;
	//int GridHeight = 20;
	//int NumOfLevels = 4;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("InitGameState")));
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
