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

public:
	ATBSGameMode(const FObjectInitializer& ObjectInitializer);

	void InitGameState();
	void PostLogin(APlayerController* PlayerController);
};
