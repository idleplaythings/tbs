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
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("unit")));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, (*It)->UnitClass->GetDescription());

		ATBSUnit* UnitActor = GetWorld()->SpawnActor<ATBSUnit>((*It)->UnitClass);;
		FCoordinateLocations Locations = GridUI->GetCoordinateLocations((*It)->Coordinates);
		UnitActor->SetActorLocation(Locations.Center);
	}
}
