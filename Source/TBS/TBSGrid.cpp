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

TMap<FString, TArray<FProp*>>::TConstIterator ATBSGrid::GetPropsIterator()
{
	return Props.CreateConstIterator();
}

TArray<FUnit*>::TConstIterator ATBSGrid::GetUnitIterator()
{
	return Units.CreateConstIterator();
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

void ATBSGrid::AddUnit(FUnit* Unit)
{
	Units.Add(Unit);
}
