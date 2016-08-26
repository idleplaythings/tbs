// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSGrid.h"
#include "TBSGridUI.h"
#include "TBSUnitManager.generated.h"

struct MovementCommand
{
	ATBSUnit* Unit;
	FIntVector Movement;
	FVector TargetWorldCoordinates;

	MovementCommand()
	{

	}

	MovementCommand(ATBSUnit* InUnit, FIntVector InMovement, FVector InTargetWorldCoordinates)
	{
		Unit = InUnit;
		Movement = InMovement;
		TargetWorldCoordinates = InTargetWorldCoordinates;
	}
};

UCLASS()
class TBS_API ATBSUnitManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBSUnitManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Initialise(ATBSGrid* Grid, ATBSGridUI* GridUI);
	void ResetUnit(ATBSUnit* Unit);
	void ResetUnits();
	void MoveUnit(ATBSUnit* Unit, FIntVector Movements);

private:
	ATBSGrid* Grid;
	ATBSGridUI* GridUI;
	bool IsProcessingMovement;
	MovementCommand CurrentMovement;
	FVector CurrentMovementTarget;
	TQueue<MovementCommand> MovementCommandQueue;
	bool ConsumeMovementCommand(MovementCommand &MovementCommand);
	void StartMovement();
	void MoveUnit(float DeltaTime);
};
