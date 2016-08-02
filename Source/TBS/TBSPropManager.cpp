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

void ATBSPropManager::RenderProps()
{
	for (auto It = Grid->Props.CreateConstIterator(); It; ++It)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, It.Key());

		for (auto& Prop : It.Value())
		{			
			ATBSProp* PropActor = GetWorld()->SpawnActor<ATBSProp>(Prop->PropClass);
			FCoordinateLocations Locations = GridUI->GetCoordinateLocations(Prop->Coordinates);

			switch (Prop->Slot)
			{
			case ETileSlot::NW:
				PropActor->SetActorLocation(Locations.NW);
				break;
			case ETileSlot::N:
				PropActor->SetActorLocation(Locations.N);
				break;
			case ETileSlot::NE:
				PropActor->SetActorLocation(Locations.NE);
				break;
			case ETileSlot::E:
				PropActor->SetActorLocation(Locations.E);
				break;
			case ETileSlot::SE:
				PropActor->SetActorLocation(Locations.SE);
				break;
			case ETileSlot::S:
				PropActor->SetActorLocation(Locations.S);
				break;
			case ETileSlot::SW:
				PropActor->SetActorLocation(Locations.SW);
				break;
			case ETileSlot::W:
				PropActor->SetActorLocation(Locations.W);
				break;
			default:
				PropActor->SetActorLocation(Locations.Center);
			}

			PropActor->SetActorRotation(Prop->Rotation);
		}
	}
}

