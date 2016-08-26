// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSProp_Wall01.h"
#include "TBSPropFactory.h"


// Sets default values
ATBSPropFactory::ATBSPropFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATBSPropFactory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBSPropFactory::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

ATBSProp* ATBSPropFactory::CreateWall(FIntVector Coordinates, FRotator Rotation)
{
	//FProp Wall;
	//Wall.PropClass = ATBSProp_Wall01::StaticClass();
	//Wall.Coordinates = Coordinates;
	//Wall.Slot = Slot;
	//Wall.Rotation = Rotation;
	//Wall.BlocksAccess = true;
	//return Wall;

	ATBSProp_Wall01* Wall = GetWorld()->SpawnActor<ATBSProp_Wall01>(ATBSProp_Wall01::StaticClass());
	Wall->GameCoordinates = Coordinates;
	Wall->BlocksAccess = true;
	Wall->SetActorRotation(Rotation);
	return Wall;
}