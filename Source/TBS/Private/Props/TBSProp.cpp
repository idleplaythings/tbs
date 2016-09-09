// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "UnrealNetwork.h"
#include "TBSProp.h"


// Sets default values
ATBSProp::ATBSProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;

	bAlwaysRelevant = false;
	bCanBeDamaged = false;
	bCollideWhenPlacing = false;
	bEditable = false;
	bLockLocation = true;
	bReplicateMovement = false;

	SetActorEnableCollision(false);
}

// Called when the game starts or when spawned
void ATBSProp::BeginPlay()
{
	Super::BeginPlay();
	
	ScalePropMesh();
}

// Called every frame
void ATBSProp::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATBSProp::ScalePropMesh()
{
	
}

void ATBSProp::RecalculateCoordinates()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Recalculating coordinates, dimensions (%i, %i, %i)"), Dimensions.X, Dimensions.Y, Dimensions.Z));

	TArray<FIntVector> NewCoordinates;

	int32 XStep = Dimensions.X % 2 == 0 ? -5 : 0;
	int32 YStep = Dimensions.Y % 2 == 0 ? -5 : 0;

	FIntVector Origin = FIntVector(
		GameCoordinates.X + XStep - (FMath::CeilToInt((float)Dimensions.X / 2) * 10 - 10),
		GameCoordinates.Y + YStep - (FMath::CeilToInt((float)Dimensions.Y / 2) * 10 - 10),
		GameCoordinates.Z
	);

	for (int32 X = 0; X <= Dimensions.X * 10; X = X + 10)
	{
		for (int32 Y = 0; Y <= Dimensions.Y * 10; Y = Y + 10)
		{
			for (int32 Z = 0; Z <= Dimensions.Z * 10; Z = Z + 10)
			{
				NewCoordinates.Add(FIntVector(
					Origin.X + X,
					Origin.Y + Y,
					Origin.Z + Z
				));
			}
		}
	}

	GameCoordinatesOccupied = NewCoordinates;
}