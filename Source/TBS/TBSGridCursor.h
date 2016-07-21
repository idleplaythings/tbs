// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSPlayerController.h"
#include "TBSGridCursor.generated.h"

UCLASS(Blueprintable)
class TBS_API ATBSGridCursor : public AActor
{
	GENERATED_BODY()
	
public:	
	float CursorUpdateFrequency = 1;
	float TimeSinceLastCursorUpdate = 0;

	ATBSPlayerController* PlayerController;

	// Sets default values for this actor's properties
	ATBSGridCursor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
};
