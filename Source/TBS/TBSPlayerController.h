// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "TBSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TBS_API ATBSPlayerController : public APlayerController
{
	GENERATED_BODY()

	ATBSPlayerController(const FObjectInitializer& ObjectInitializer);


private:
	void EnableMouse();
};
