// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSLargeUnit.h"


// Sets default values
ATBSLargeUnit::ATBSLargeUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Dimensions = FIntVector(3, 3, 6);
}

// Called when the game starts or when spawned
void ATBSLargeUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBSLargeUnit::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

