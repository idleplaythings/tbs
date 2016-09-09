// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSProp_Wall01.h"
#include "TBSProp_Block.h"
#include "TBSPropFactory.h"


// Sets default values
ATBSPropFactory::ATBSPropFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

ATBSProp* ATBSPropFactory::CreateBlock(FIntVector Coordinates, FIntVector Dimensions, FRotator Rotation)
{
	ATBSProp_Block* Block = GetWorld()->SpawnActor<ATBSProp_Block>(ATBSProp_Block::StaticClass());
	Block->GameCoordinates = Coordinates;
	Block->Dimensions = Dimensions;
	Block->BlocksAccess = true;
	Block->Rotation = Rotation;
	Block->RecalculateCoordinates();
	return Block;
}

//ATBSProp* ATBSPropFactory::CreateWall(FIntVector Coordinates, FRotator Rotation)
//{
//	ATBSProp_Wall01* Wall = GetWorld()->SpawnActor<ATBSProp_Wall01>(ATBSProp_Wall01::StaticClass());
//	Wall->GameCoordinates = Coordinates;
//	Wall->BlocksAccess = true;
//	Wall->SetActorRotation(Rotation);
//	return Wall;
//}