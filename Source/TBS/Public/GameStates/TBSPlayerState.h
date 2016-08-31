// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "TBSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TBS_API ATBSPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;
	void PostInitializeComponents() override;
	void ClientInitialize(AController* Controller);
};
