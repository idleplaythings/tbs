// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "UnrealNetwork.h"
#include "TBSGrid.h"


// Sets default values
ATBSGrid::ATBSGrid()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void ATBSGrid::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	DOREPLIFETIME(ATBSGrid, GridDimensions);
	DOREPLIFETIME(ATBSGrid, Units);
}

// Called when the game starts or when spawned
void ATBSGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATBSGrid::InitialiseGrid(FIntVector InGridDimensions)
{
	GridDimensions = InGridDimensions;
}

TArray<ATBSUnit*>::TIterator ATBSGrid::GetUnitIterator()
{
	return Units.CreateIterator();
}

void ATBSGrid::AddProp(FProp &Prop)
{
	if (Prop.Id == 0)
	{
		Prop.Id = NextPropId++;
	}

	PropMap.Add(Prop.Id, Prop);

	for (auto& Coordinates : GetOccupiedCoordinates(Prop))
	{
		TArray<uint32>* PropIds = OccupiedCoordinatesMap.Find(Coordinates);

		if (!PropIds)
		{
			TArray<uint32> PropIdsArray;
			PropIdsArray.Add(Prop.Id);
			OccupiedCoordinatesMap.Add(Coordinates, PropIdsArray);
		}
		else
		{
			PropIds->Add(Prop.Id);
		}
	}

	NumOfProps++;
}

TArray<FIntVector> ATBSGrid::GetOccupiedCoordinates(FProp Prop)
{
	TArray<FIntVector> Tiles;
	FIntVector Dimensions = Prop.Dimensions;

	if (Prop.Rotation == 90 || Prop.Rotation == 270)
	{
		Dimensions.X = Prop.Dimensions.Y;
		Dimensions.Y = Prop.Dimensions.X;
	}

	int32 XStep = Dimensions.X % 2 == 0 ? -5 : 0;
	int32 YStep = Dimensions.Y % 2 == 0 ? -5 : 0;

	FIntVector Origin = FIntVector(
		Prop.Coordinates.X + XStep - (FMath::CeilToInt((float)Dimensions.X / 2) * 10 - 10),
		Prop.Coordinates.Y + YStep - (FMath::CeilToInt((float)Dimensions.Y / 2) * 10 - 10),
		Prop.Coordinates.Z
	);

	for (int32 X = 0; X < Dimensions.X * 10; X = X + 10)
	{
		for (int32 Y = 0; Y < Dimensions.Y * 10; Y = Y + 10)
		{
			for (int32 Z = 0; Z < Dimensions.Z * 10; Z = Z + 10)
			{
				Tiles.Add(FIntVector(
					Origin.X + X,
					Origin.Y + Y,
					Origin.Z + Z
				));
			}
		}
	}

	return Tiles;
}

TArray<FIntVector> ATBSGrid::GetOccupiedCoordinates(ATBSUnit* Unit)
{
	TArray<FIntVector> Tiles;
	FIntVector Dimensions = Unit->Dimensions;

	int32 XStep = Dimensions.X % 2 == 0 ? -5 : 0;
	int32 YStep = Dimensions.Y % 2 == 0 ? -5 : 0;

	FIntVector Origin = FIntVector(
		Unit->GameCoordinates.X + XStep - (FMath::CeilToInt((float)Dimensions.X / 2) * 10 - 10),
		Unit->GameCoordinates.Y + YStep - (FMath::CeilToInt((float)Dimensions.Y / 2) * 10 - 10),
		Unit->GameCoordinates.Z
	);

	for (int32 X = 0; X < Dimensions.X * 10; X = X + 10)
	{
		for (int32 Y = 0; Y < Dimensions.Y * 10; Y = Y + 10)
		{
			for (int32 Z = 0; Z < Dimensions.Z * 10; Z = Z + 10)
			{
				Tiles.Add(FIntVector(
					Origin.X + X,
					Origin.Y + Y,
					Origin.Z + Z
				));
			}
		}
	}

	return Tiles;
}

void ATBSGrid::RemovePropsAt(FIntVector Coordinates)
{
	TArray<uint32>* PropIds = OccupiedCoordinatesMap.Find(Coordinates);

	if (PropIds)
	{
		NumOfProps -= PropIds->Num();

		for (auto& PropId : *PropIds)
		{
			PropMap.Remove(PropId);
		}
	}

	OccupiedCoordinatesMap.Remove(Coordinates);
}

void ATBSGrid::RemovePropById(uint32 PropId)
{
	FProp* Prop = PropMap.Find(PropId);

	if (Prop)
	{
		for (auto& Coordinates : GetOccupiedCoordinates(*Prop))
		{
			TArray<uint32>* PropIds = OccupiedCoordinatesMap.Find(Prop->Coordinates);

			if (PropIds)
			{
				PropIds->Remove(PropId);
			}
		}
	}

	PropMap.Remove(PropId);
}

TArray<FProp> ATBSGrid::GetPropsAt(FIntVector Coordinates)
{
	TArray<FProp> Props;

	TArray<uint32>* PropIds = OccupiedCoordinatesMap.Find(Coordinates);

	if (PropIds)
	{
		for (auto& PropId : *PropIds)
		{
			FProp* FoundProp = PropMap.Find(PropId);

			if (FoundProp)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Found prop %i (%i, %i, %i"), FoundProp->Id, FoundProp->Coordinates.X, FoundProp->Coordinates.Y, FoundProp->Coordinates.Z));
				Props.Add(*FoundProp);
			}			
		}
	}

	return Props;
}

void ATBSGrid::AddUnit(ATBSUnit* Unit)
{
	Units.Add(Unit);
}

uint32 ATBSGrid::PropCount()
{
	return NumOfProps;
}

ATBSUnit* ATBSGrid::SelectUnit(FIntVector Coordinates)
{
	for (auto &Unit : Units)
	{
		if (Unit == nullptr)
		{
			continue;
		}

		if (Unit->GameCoordinates == Coordinates)
		{
			return Unit;
		}

		for (auto& OccupiedGoordinates : GetOccupiedCoordinates(Unit))
		{
			if (OccupiedGoordinates == Coordinates)
			{
				return Unit;
			}
		}
	}

	return nullptr;
}

TArray<FIntVector> ATBSGrid::GetNeighbours(FIntVector Coordinates)
{
	TArray<FIntVector> Neighbours;

	for (int X = Coordinates.X - 10; X <= Coordinates.X + 10; X= X+10)
	{
		for (int Y = Coordinates.Y - 10; Y <= Coordinates.Y + 10; Y= Y+10)
		{
			//for (int Z = Coordinates.Z - 1; Z <= Coordinates.Z + 1; Z++)
			//{
				Neighbours.Add(FIntVector(X, Y, Coordinates.Z));
			//}
		}
	}

	return Neighbours.FilterByPredicate([this, Coordinates](const FIntVector& Neighbour) {
		return Neighbour != Coordinates
			&& Neighbour.X >= 0 && Neighbour.X < this->GridDimensions.X
			&& Neighbour.Y >= 0 && Neighbour.Y < this->GridDimensions.Y
			&& Neighbour.Z >= 0 && Neighbour.Z < this->GridDimensions.Z;
	});
}

bool ATBSGrid::IsAccessible(FIntVector Coordinates)
{
	TArray<uint32>* PropIds = OccupiedCoordinatesMap.Find(Coordinates);
	bool Accessible = true;
	
	if (PropIds)
	{
		for (auto& PropId : *PropIds)
		{
			FProp* Prop = PropMap.Find(PropId);

			if (Prop && Prop->BlocksAccess)
			{
				Accessible = false;
				break;
			}
		}
	}

	return Accessible;
}

void ATBSGrid::ReindexProps_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Reindexing Props.")));

	// Todo...
}

bool ATBSGrid::GetLineOfFireBetweenUnits(ATBSUnit* UnitA, ATBSUnit* UnitB, TArray<FIntVector> &OutTrace)
{
	// Naive mutual LoF implementation, this is a deep and nasty loop
	bool HasLof = false;

	for (auto& Coordinates0 : GetOccupiedCoordinates(UnitA))
	{
		for (auto& Coordinates1 : GetOccupiedCoordinates(UnitB))
		{
			TArray<FIntVector> Trace;

			if (CanDrawLineOfFire(Coordinates0, Coordinates1, Trace))
			{
				OutTrace = Trace;
				HasLof = true;
				break;
			}
		}

		if (HasLof)
		{
			break;
		}
	}

	return HasLof;
}

void ATBSGrid::ReindexUnits_Implementation()
{
	if (HasAuthority())
	{
		TArray<ATBSUnit*> Player0Units = GetUnitsByPlayer(0);
		TArray<ATBSUnit*> Player1Units = GetUnitsByPlayer(1);

		// Track visible units. A unit is visible if anyone on the opposing team sees it.
		TArray<ATBSUnit*> Player0VisibleUnits;
		TArray<ATBSUnit*> Player1VisibleUnits;

		//uint32 NumberOfTraces = 0;

		//TraceRenderer->ClearTraces();

		for (auto& Unit0 : Player0Units)
		{
			for (auto& Unit1 : Player1Units)
			{
				//// Naive mutual LoF implementation, this is a deep and nasty loop
				//bool HasLof = false;

				//for (auto& Coordinates0 : GetOccupiedCoordinates(Unit0))
				//{
				//	for (auto& Coordinates1 : GetOccupiedCoordinates(Unit1))
				//	{
				//		NumberOfTraces++;
				//		if (CanDrawLineOfFire(Coordinates0, Coordinates1))
				//		{
				//			//TraceRenderer->RenderTrace(Coordinates0, Coordinates1, Trace(Coordinates0, Coordinates1));

				//			HasLof = true;
				//			break;
				//		}
				//	}

				//	if (HasLof)
				//	{
				//		break;
				//	}
				//}

				TArray<FIntVector> Trace;
				if (GetLineOfFireBetweenUnits(Unit0, Unit1, Trace))
				{
					// Mutual line of fire
					Player0VisibleUnits.AddUnique(Unit0);
					Player1VisibleUnits.AddUnique(Unit1);
				}
			}
		}

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%i LoF traces"), NumberOfTraces));

		// Now Loop through each player's visible units and update visibility only once per reindex...

		for (auto& Unit0 : Player0Units)
		{
			if (Player0VisibleUnits.Contains(Unit0))
			{
				Unit0->SeenByPlayers.AddUnique(1);
			}
			else
			{
				if (Unit0->SeenByPlayers.Contains(1))
				{
					Unit0->SeenByPlayers.Remove(1);
					// Unit was visible, but now isn't. Close actor replication channel
					// to hide it immediately (rather than after 5 seconds)
					OnActorNoLongerVisible.Broadcast(1, Unit0);					
				}
			}
		}

		for (auto& Unit1 : Player1Units)
		{
			if (Player1VisibleUnits.Contains(Unit1))
			{
				Unit1->SeenByPlayers.AddUnique(0);
			}
			else
			{
				if (Unit1->SeenByPlayers.Contains(0))
				{
					Unit1->SeenByPlayers.Remove(0);
					// Unit was visible, but now isn't. Close actor replication channel
					// to hide it immediately (rather than after 5 seconds)
					OnActorNoLongerVisible.Broadcast(0, Unit1);					
				}
			}
		}
	}	
}

bool ATBSGrid::CanDrawLineOfFire(FIntVector Start, FIntVector End, TArray<FIntVector> &OutTrace)
{
	TArray<FIntVector> TraceResult = Trace(Start, End);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Tracing (%i, %i, %i) -> (%i, %i, %i)"), Start.X, Start.Y, Start.Z, End.X, End.Y, End.Z));

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Raw trace length %i"), TraceResult.Num()));

	OutTrace.Empty();

	for (auto& Step : TraceResult)
	{
		OutTrace.AddUnique(
			FIntVector(
				(int32)Step.X / 10 * 10,
				(int32)Step.Y / 10 * 10,
				(int32)Step.Z / 10 * 10
			)
		);
	}

	for (auto& Coordinate : OutTrace)
	{
		if (!IsAccessible(Coordinate))
		{
			return false;
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Processed trace length %i"), OutTrace.Num()));

	return true;
}

TArray<FIntVector> ATBSGrid::Trace(FIntVector Start, FIntVector End)
{
	TArray<FIntVector> Trace;

	// http://members.chello.at/easyfilter/bresenham.html
	int32 x0 = Start.X;
	int32 y0 = Start.Y;
	int32 z0 = Start.Z;

	int32 x1 = End.X;
	int32 y1 = End.Y;
	int32 z1 = End.Z;

	int32 dx = FMath::Abs(x1 - x0);
	int32 sx = x0 < x1 ? 1 : -1;
	int32 dy = FMath::Abs(y1 - y0);
	int32 sy = y0 < y1 ? 1 : -1;
	int32 dz = FMath::Abs(z1 - z0);
	int32 sz = z0 < z1 ? 1 : -1;
	int32 dm = FMath::Max3(dx, dy, dz);
	int32 i = dm; /* maximum difference */

	x1 = y1 = z1 = dm / 2; /* error offset */

	for (;;) {  /* loop */
		//int32 TempX, TempY;

		//int32 ModX = x0 % 10;
		//int32 ModY = y0 % 10;

		//if (ModX == 0 || ModX == 5)
		//{
		//	TempX = x0;
		//}
		//else if (ModX < 5)
		//{
		//	TempX = x0 - ModX;
		//}
		//else
		//{
		//	TempX = x0 + 1 + ModX;
		//}

		//if (ModY == 0 || ModY == 5)
		//{
		//	TempY = y0;
		//}
		//else if (ModY < 5)
		//{
		//	TempY = y0 - ModY;
		//}
		//else
		//{
		//	TempY = y0 + 1 + ModY;
		//}

		Trace.Add(FIntVector(x0, y0, z0));
		//Trace.Add(FIntVector(TempX, TempY, z0));

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Tracing (%i, %i, %i)"), x0, y0, z0));
		if (i-- == 0) break;
		x1 -= dx; if (x1 < 0) { x1 += dm; x0 += sx; }
		y1 -= dy; if (y1 < 0) { y1 += dm; y0 += sy; }
		z1 -= dz; if (z1 < 0) { z1 += dm; z0 += sz; }
	}

	return Trace;
}

TArray<ATBSUnit*> ATBSGrid::GetUnitsByPlayer(int32 PlayerNumber)
{
	return Units.FilterByPredicate([&](const ATBSUnit* Unit) {
		return Unit->PlayerNumber == PlayerNumber;
	});
}

TArray<FIntVector> ATBSGrid::GetAccessibleNeighbours(FIntVector Coordinates)
{
	TArray<FIntVector> Neighbours = GetNeighbours(Coordinates);

	return Neighbours.FilterByPredicate([&](const FIntVector& Neighbour) {
		return IsAccessible(Neighbour);
	});
}
