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
		if (ConsumeMovementCommand(CurrentMovement))
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
		(*It)->UnitObject = GetWorld()->SpawnActor<ATBSUnit>((*It)->UnitClass);
		FCoordinateLocations Locations = GridUI->GetCoordinateLocations((*It)->Coordinates);
		(*It)->UnitObject->SetActorLocation(Locations.Center);
		//(*It).UnitObject = UnitActor;
	}
}

void ATBSUnitManager::MoveUnit(FUnit* Unit, FMovement Movement)
{
	MovementCommandQueue.Enqueue(FMovementCommand(Unit, Movement));
}

bool ATBSUnitManager::ConsumeMovementCommand(FMovementCommand &MovementCommand)
{
	return MovementCommandQueue.Dequeue(MovementCommand);
}

void ATBSUnitManager::StartMovement()
{
	IsProcessingMovement = true;
	CurrentMovement.Unit->Coordinates = CurrentMovement.Movement.TargetGameCoordinates;
}

void ATBSUnitManager::MoveUnit(float DeltaTime)
{
	if (IsProcessingMovement)
	{
		FVector RemainingMovement = CurrentMovement.Movement.TargetWorldCoordinates - CurrentMovement.Unit->UnitObject->GetActorLocation();

		ATBSUnit* UnitActor = CurrentMovement.Unit->UnitObject;

		UnitActor->Speed = FMath::Clamp(UnitActor->Speed + UnitActor->Acceleration, 0.0f, UnitActor->MaxSpeed);
	
		if (RemainingMovement.Size() <= UnitActor->Speed)
		{
			UnitActor->SetActorLocation(CurrentMovement.Movement.TargetWorldCoordinates);
			IsProcessingMovement = false;

			if (MovementCommandQueue.IsEmpty())
			{
				UnitActor->Speed = 0;
			}
		}
		else
		{
			RemainingMovement.Normalize();
			UnitActor->AddActorWorldOffset(RemainingMovement * UnitActor->Speed);
		}

		FRotator TargetRotation = FRotationMatrix::MakeFromX(RemainingMovement).Rotator();
		FRotator CurrentRotation = UnitActor->GetActorRotation();

		float Diff = TargetRotation.Yaw - CurrentRotation.Yaw;

		if (FMath::Abs(Diff) < 5 || FMath::Abs(FMath::Abs(Diff) - 360) < 5)
		{
			UnitActor->SetActorRotation(TargetRotation);
		}
		else
		{
			if (Diff > 0 && Diff <= 180)
			{
				UnitActor->AddActorWorldRotation(FRotator(0.0, UnitActor->TurningSpeed, 0.0));
			}
			else
			{
				UnitActor->AddActorWorldRotation(FRotator(0.0, UnitActor->TurningSpeed * -1, 0.0));
			}
		}
	}
}
