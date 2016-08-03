// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSUnitFactory.h"


// Sets default values
ATBSUnitFactory::ATBSUnitFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UBlueprint> UnitBP(TEXT("Blueprint'/Game/Blueprints/Units/BP_TBSUnit.BP_TBSUnit'"));

	if (UnitBP.Object)
	{
		UnitBPClass = (UClass*)UnitBP.Object->GeneratedClass;
	}	
}

// Called when the game starts or when spawned
void ATBSUnitFactory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBSUnitFactory::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

FUnit ATBSUnitFactory::CreateUnit(FIntVector Coordinates, FRotator Rotation)
{
	FUnit Unit;
	Unit.UnitClass = UnitBPClass;
	Unit.Coordinates = Coordinates;
	Unit.Rotation = Rotation;
	return Unit;
}