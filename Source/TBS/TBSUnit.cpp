// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSUnit.h"


// Sets default values
ATBSUnit::ATBSUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATBSUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBSUnit::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATBSUnit::AddGridMovement(FMovement Movement)
{
	MovementQueue.Enqueue(Movement);
}

bool ATBSUnit::ConsumeGridMovement(FMovement &Movement)
{
	return MovementQueue.Dequeue(Movement);
}
