// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "TBSGameMode.generated.h"

/**
 *
 */
UCLASS()
class TBS_API ATBSGameMode : public AGameMode
{
	GENERATED_BODY()

	ATBSGameMode(const FObjectInitializer& ObjectInitializer);

public:
	float TileSize = 100;
	int GridWidth = 20;
	int GridHeight = 20;
	int NumOfLevels = 4;
	float FloorHeight = 200;

	void InitGame(
		const FString & MapName,
		const FString & Options,
		FString & ErrorMessage
	);
	void InitGameState();
	FString  InitNewPlayer(
		class APlayerController * NewPlayerController,
		const TSharedPtr < const FUniqueNetId > & UniqueId,
		const FString & Options,
		const FString & Portal
	);
};
