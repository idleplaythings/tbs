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
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSUnitManager::Tick (%i, %i, %i)"), CurrentMovement.Movement.X, CurrentMovement.Movement.Y, CurrentMovement.Movement.Z));
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

void ATBSUnitManager::ResetUnits()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Rendering units")));

	for (auto It = Grid->GetUnitIterator(); It; ++It)
	{
		//(*It)->UnitObject = GetWorld()->SpawnActor<ATBSUnit>((*It)->UnitClass);
		FCoordinateLocations Locations = GridUI->GetCoordinateLocations((*It)->GameCoordinates);
		(*It)->SetActorLocation(Locations.Center);
		//(*It).UnitObject = UnitActor;
	}
}

void ATBSUnitManager::ResetUnit(ATBSUnit* Unit)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Rendering units")));

	FCoordinateLocations Locations = GridUI->GetCoordinateLocations(Unit->GameCoordinates);
	Unit->SetActorLocation(Locations.Center);
}

void ATBSUnitManager::MoveUnit(ATBSUnit* Unit, FIntVector Movement)
{
	FCoordinateLocations Locations = GridUI->GetCoordinateLocations(Movement);
	MovementCommandQueue.Enqueue(MovementCommand(Unit, Movement, Locations.Center));
}

bool ATBSUnitManager::ConsumeMovementCommand(MovementCommand &MovementCommand)
{
	return MovementCommandQueue.Dequeue(MovementCommand);
}

void ATBSUnitManager::StartMovement()
{
	IsProcessingMovement = true;
	CurrentMovement.Unit->GameCoordinates = CurrentMovement.Movement;
	Grid->ReindexUnits();
}

void ATBSUnitManager::MoveUnit(float DeltaTime)
{	
	if (IsProcessingMovement)
	{
		FVector RemainingMovement = CurrentMovement.TargetWorldCoordinates - CurrentMovement.Unit->GetActorLocation();

		ATBSUnit* Unit = CurrentMovement.Unit;

		Unit->Speed = FMath::Clamp(Unit->Speed + Unit->Acceleration, 0.0f, Unit->MaxSpeed);
	
		if (RemainingMovement.Size() <= Unit->Speed)
		{
			//Unit->SetActorLocation(CurrentMovement.Movement.TargetWorldCoordinates);
			Unit->SetActorLocation(CurrentMovement.TargetWorldCoordinates);
			IsProcessingMovement = false;

			if (MovementCommandQueue.IsEmpty())
			{
				Unit->Speed = 0;
			}			
		}
		else
		{
			RemainingMovement.Normalize();
			Unit->SetActorLocation(Unit->GetActorLocation() + RemainingMovement * Unit->Speed);
		}

		FRotator TargetRotation = FRotationMatrix::MakeFromX(RemainingMovement).Rotator();
		FRotator CurrentRotation = Unit->GetActorRotation();

		float Diff = TargetRotation.Yaw - CurrentRotation.Yaw;

		if (FMath::Abs(Diff) < 5 || FMath::Abs(FMath::Abs(Diff) - 360) < 5)
		{
			Unit->SetActorRotation(TargetRotation);
		}
		else
		{
			if (Diff > 0 && Diff <= 180)
			{
				Unit->AddActorWorldRotation(FRotator(0.0, Unit->TurningSpeed, 0.0));
			}
			else
			{
				Unit->AddActorWorldRotation(FRotator(0.0, Unit->TurningSpeed * -1, 0.0));
			}
		}
	}
}
