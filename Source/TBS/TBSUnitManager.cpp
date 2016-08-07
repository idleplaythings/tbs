// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSUnit.h"
#include "TBSUnitManager.h"

// Sets default values
ATBSUnitManager::ATBSUnitManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATBSUnitManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBSUnitManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (!IsProcessingMovement)
	{
		if (ConsumeMovementCommand(CurrentMovementCommand))
		{
			StartMovement();
		}
	}

	MoveUnit(DeltaTime);
}

void ATBSUnitManager::Initialise(ATBSGrid* InGrid, ATBSGridUI* InGridUI)
{
	Grid = InGrid;
	GridUI = InGridUI;
}

void ATBSUnitManager::RenderUnits()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Rendering units")));

	for (auto It = Grid->GetUnitIterator(); It; ++It)
	{
		(*It).UnitObject = GetWorld()->SpawnActor<ATBSUnit>((*It).UnitClass);
		FCoordinateLocations Locations = GridUI->GetCoordinateLocations((*It).Coordinates);
		(*It).UnitObject->SetActorLocation(Locations.Center);
		//(*It).UnitObject = UnitActor;
	}
}

void ATBSUnitManager::MoveUnit(FUnit &Unit, FMovement Movement)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Unit addr (%#010x)"), &Unit));
	MovementCommandQueue.Enqueue(FMovementCommand(Unit, Movement));
}

bool ATBSUnitManager::ConsumeMovementCommand(FMovementCommand &MovementCommand)
{
	return MovementCommandQueue.Dequeue(MovementCommand);
}

void ATBSUnitManager::StartMovement()
{
	IsProcessingMovement = true;
	CurrentMovementCommand.Unit.Coordinates = CurrentMovementCommand.Movement.TargetGameCoordinates;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Current unit game coords (%i, %i, %i)"), CurrentMovementCommand.Unit.Coordinates.X, CurrentMovementCommand.Unit.Coordinates.Y, CurrentMovementCommand.Unit.Coordinates.Z));
}

void ATBSUnitManager::MoveUnit(float DeltaTime)
{
	if (IsProcessingMovement)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Performing movement")));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Current unit game coords (%i, %i, %i)"), CurrentMovementCommand.Unit.Coordinates.X, CurrentMovementCommand.Unit.Coordinates.Y, CurrentMovementCommand.Unit.Coordinates.Z));

		AActor* UnitActor = CurrentMovementCommand.Unit.UnitObject;

		UnitActor->AddActorWorldOffset(CurrentMovementCommand.Movement.TargetWorldCoordinates - UnitActor->GetActorLocation());

		IsProcessingMovement = false;
	}
}
