// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "UnrealNetwork.h"
#include "TBSGrid.h"


// Sets default values
ATBSGrid::ATBSGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void ATBSGrid::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	DOREPLIFETIME(ATBSGrid, GridDimensions);
	DOREPLIFETIME(ATBSGrid, Units);
	DOREPLIFETIME(ATBSGrid, Props);
}

// Called when the game starts or when spawned
void ATBSGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
//void ATBSGrid::Tick( float DeltaTime )
//{
//	Super::Tick( DeltaTime );
//
//}

void ATBSGrid::InitialiseGrid(FIntVector InGridDimensions)
{
	GridDimensions = InGridDimensions;
}

TArray<ATBSProp*>::TIterator ATBSGrid::GetPropsIterator()
{
	return Props.CreateIterator();
}

TArray<ATBSUnit*>::TIterator ATBSGrid::GetUnitIterator()
{
	return Units.CreateIterator();
}

void ATBSGrid::AddProp(ATBSProp* Prop)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Adding a prop")));
	Props.Add(Prop);
}

void ATBSGrid::AddUnit(ATBSUnit* Unit)
{
	Units.Add(Unit);
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

		for (auto& OccupiedGoordinates : Unit->GameCoordinatesOccupied)
		{
			if (OccupiedGoordinates == Coordinates)
			{
				return Unit;
			}
		}
	}

	return nullptr;
}

ATBSProp* ATBSGrid::SelectProp(FIntVector Coordinates)
{
	for (auto &Prop : Props)
	{
		if (Prop == nullptr)
		{
			continue;
		}

		if (Prop->GameCoordinates == Coordinates)
		{
			return Prop;
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
	ATBSProp** FoundProp = PropsIndex.Find(Coordinates);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Looking at coordinates (%i, %i, %i)"), Coordinates.X, Coordinates.Y, Coordinates.Z));

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Prop index length %i"), PropsIndex.Num()));
	
	if (FoundProp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Prop found from index")));

		if ((*FoundProp)->BlocksAccess)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Prop is blocking")));

			return false;
		}
	}

	return true;
}

void ATBSGrid::ReindexProps_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Reindexing Props. Prop array length %i"), Props.Num()));

	PropsIndex.Empty();

	for (auto& Prop : Props)
	{
		PropsIndex.Add(Prop->GameCoordinates, Prop);
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Prop index length %i"), PropsIndex.Num()));
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

		for (auto& Unit0 : Player0Units)
		{
			for (auto& Unit1 : Player1Units)
			{
				if (CanDrawLineOfFire(Unit0->GameCoordinates, Unit1->GameCoordinates))
				{
					// Mutual line of fire
					Player0VisibleUnits.AddUnique(Unit0);
					Player1VisibleUnits.AddUnique(Unit1);
				}
			}
		}

		// Now Loop through each player's visible units and update visibilityh only once per reindex...

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

bool ATBSGrid::CanDrawLineOfFire(FIntVector Start, FIntVector End)
{
	for (auto& Coordinate : Trace(Start, End))
	{
		if (!IsAccessible(Coordinate))
		{
			return false;
		}
	}

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
		Trace.Add(FIntVector(x0, y0, z0));
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

	//return Neighbours.FilterByPredicate([this, Coordinates](const FIntVector& Neighbour) {
	//	bool SameX = Coordinates.X == Neighbour.X;
	//	bool SameY = Coordinates.Y == Neighbour.Y;

	//	// If only either X or Y coordinates change, neighbour is directly adjacent
	//	// and we only need to consider props blocking direct access
	//	if (SameX ? !SameY : SameY)
	//	{
	//		if (SameX)
	//		{
	//			if (Neighbour.Y > Coordinates.Y)
	//			{					
	//				return !IsTileAccessBlockedByProp(Coordinates, ETileSlot::S)
	//					&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::N);
	//			}
	//			else
	//			{	
	//				return !IsTileAccessBlockedByProp(Coordinates, ETileSlot::N)
	//					&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::S);
	//			}
	//		}
	//		else {
	//			if (Neighbour.X > Coordinates.X)
	//			{
	//				return !IsTileAccessBlockedByProp(Coordinates, ETileSlot::E)
	//					&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::W);
	//			}
	//			else
	//			{
	//				return !IsTileAccessBlockedByProp(Coordinates, ETileSlot::W)
	//					&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::E);
	//			}
	//		}
	//	}
	//	// Otherwise neighbour is diagonally adjacent, and we need to consider blocking access
	//	// in neighbouring tiles as well
	//	else
	//	{
	//		if (Neighbour.X > Coordinates.X)
	//		{
	//			if (Neighbour.Y > Coordinates.Y)
	//			{
	//				FIntVector OneEast = Coordinates + FIntVector(1, 0, 0);
	//				FIntVector OneSouth = Coordinates + FIntVector(0, 1, 0);

	//				return !IsTileAccessBlockedByProp(Coordinates, ETileSlot::E)
	//					&& !IsTileAccessBlockedByProp(Coordinates, ETileSlot::S)
	//					&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::N)
	//					&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::W)
	//					&& !IsTileAccessBlockedByProp(OneEast, ETileSlot::W)
	//					&& !IsTileAccessBlockedByProp(OneEast, ETileSlot::S)
	//					&& !IsTileAccessBlockedByProp(OneSouth, ETileSlot::N)
	//					&& !IsTileAccessBlockedByProp(OneSouth, ETileSlot::E);
	//			}
	//			else {
	//				FIntVector OneEast = Coordinates + FIntVector(1, 0, 0);
	//				FIntVector OneNorth = Coordinates + FIntVector(0, -1, 0);

	//				return !IsTileAccessBlockedByProp(Coordinates, ETileSlot::E)
	//					&& !IsTileAccessBlockedByProp(Coordinates, ETileSlot::N)
	//					&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::S)
	//					&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::W)
	//					&& !IsTileAccessBlockedByProp(OneEast, ETileSlot::W)
	//					&& !IsTileAccessBlockedByProp(OneEast, ETileSlot::N)
	//					&& !IsTileAccessBlockedByProp(OneNorth, ETileSlot::S)
	//					&& !IsTileAccessBlockedByProp(OneNorth, ETileSlot::E);
	//			}
	//		}
	//		else {
	//			if (Neighbour.Y > Coordinates.Y)
	//			{
	//				FIntVector OneWest = Coordinates + FIntVector(-1, 0, 0);
	//				FIntVector OneSouth = Coordinates + FIntVector(0, 1, 0);

	//				return !IsTileAccessBlockedByProp(Coordinates, ETileSlot::W)
	//					&& !IsTileAccessBlockedByProp(Coordinates, ETileSlot::S)
	//					&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::N)
	//					&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::E)
	//					&& !IsTileAccessBlockedByProp(OneWest, ETileSlot::E)
	//					&& !IsTileAccessBlockedByProp(OneWest, ETileSlot::S)
	//					&& !IsTileAccessBlockedByProp(OneSouth, ETileSlot::N)
	//					&& !IsTileAccessBlockedByProp(OneSouth, ETileSlot::W);
	//			}
	//			else
	//			{
	//				FIntVector OneWest = Coordinates + FIntVector(-1, 0, 0);
	//				FIntVector OneNorth = Coordinates + FIntVector(0, -1, 0);					

	//				return !IsTileAccessBlockedByProp(Coordinates, ETileSlot::N)
	//					&& !IsTileAccessBlockedByProp(Coordinates, ETileSlot::W)
	//					&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::S)
	//					&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::E)
	//					&& !IsTileAccessBlockedByProp(OneWest, ETileSlot::N)
	//					&& !IsTileAccessBlockedByProp(OneWest, ETileSlot::E)
	//					&& !IsTileAccessBlockedByProp(OneNorth, ETileSlot::W)
	//					&& !IsTileAccessBlockedByProp(OneNorth, ETileSlot::S);
	//			}
	//		}
	//	}
	//	return true;
	//});

	//return Neighbours;
}
//
//bool ATBSGrid::IsTileAccessBlockedByProp(FIntVector Coordinates, ETileSlot Slot)
//{
//	TArray<ATBSProp*>* PropsInTile = Props.Find(Coordinates);
//
//	if (PropsInTile == nullptr)
//	{
//		return false;
//	}
//
//	FString Name;
//	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ETileSlot"), true);
//	if (!EnumPtr)
//	{
//		Name = FString(TEXT("Nope"));
//	}
//	else
//	{
//		Name = EnumPtr->GetDisplayNameText((int8)Slot).ToString();
//	}	
//
//	for (auto& Prop : *PropsInTile)
//	{
//
//		FString Name2;
//		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ETileSlot"), true);
//		if (!EnumPtr)
//		{
//			Name2 = FString(TEXT("Nope"));
//		}
//		else
//		{
//			Name2 = EnumPtr->GetDisplayNameText((int8)Prop.Slot).ToString();
//		}
//
//		if (Prop->Slot == Slot && Prop->BlocksAccess)
//		{
//			return true;
//		}
//	}
//
//	return false;
//}
