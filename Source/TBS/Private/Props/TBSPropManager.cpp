// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSPropManager.h"


// Sets default values
ATBSPropManager::ATBSPropManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATBSPropManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBSPropManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATBSPropManager::Initialise(ATBSGrid* InGrid, ATBSGridUI* InGridUI)
{
	Grid = InGrid;
	GridUI = InGridUI;
}

void ATBSPropManager::ResetProps()
{
	for (auto It = Grid->GetPropsIterator(); It; ++It)
	{
		ResetProp(*It);
	}
}

void ATBSPropManager::ResetProp(ATBSProp* Prop)
{
	FCoordinateLocations Locations = GridUI->GetCoordinateLocations(Prop->GameCoordinates);
	Prop->SetActorLocation(Locations.Center);
}
