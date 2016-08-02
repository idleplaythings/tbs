// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSGrid.h"


// Sets default values
ATBSGrid::ATBSGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATBSGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBSGrid::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATBSGrid::InitialiseGrid(int32 GridWidth, int32 GridHeight, int32 NumOfLevels)
{
	this->GridWidth = GridWidth;
	this->GridHeight = GridHeight;
	this->NumOfLevels = NumOfLevels;
}

void ATBSGrid::AddProp(FProp* Prop)
{
	FString Key = FString::Printf(TEXT("%i,%i,%i"), Prop->Coordinates.X, Prop->Coordinates.Y, Prop->Coordinates.Z);
	TArray<FProp*>* Result = Props.Find(Key);

	if (Result != nullptr)
	{
		Result->Add(Prop);
	}
	else
	{
		TArray<FProp*> Arr;
		Arr.Add(Prop);
		Props.Add(Key, Arr);
	}
}

void ATBSGrid::NotifyCursorLocationChange(FIntVector OldCoordinates, FIntVector NewCoordinates)
{
	HoverCoordinates = NewCoordinates;
	OnGameTileHoverEnd.Broadcast(OldCoordinates);
	OnGameTileHoverBegin.Broadcast(NewCoordinates);
}

void ATBSGrid::NotifyClick()
{
	if (HoverCoordinates == NullVector)
	{
		return;
	}

	if (SelectCoordinates != NullVector)
	{
		OnGameTileSelectEnd.Broadcast(SelectCoordinates);
	}

	SelectCoordinates = HoverCoordinates;
	OnGameTileSelectBegin.Broadcast(SelectCoordinates);
}

void ATBSGrid::NotifyCursorOffGrid()
{
	if (HoverCoordinates != NullVector)
	{
		OnGameTileHoverEnd.Broadcast(HoverCoordinates);
	}

	HoverCoordinates = NullVector;
}
