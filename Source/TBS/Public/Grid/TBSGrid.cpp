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
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Adding a prop")));
	Props.Add(Prop);
}

void ATBSGrid::AddUnit(ATBSUnit* Unit)
{
	Units.Add(Unit);
}

ATBSUnit* ATBSGrid::SelectUnit(FIntVector GameCoords)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Looking for units, number of units %i"), Units.Num()));

	for (auto &Unit : Units)
	{
		if (Unit == nullptr)
		{
			continue;
		}

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Unit coords (%i, %i, %i)"), Unit->GameCoordinates.X, Unit->GameCoordinates.Y, Unit->GameCoordinates.Z));

		if (Unit->GameCoordinates == GameCoords)
		{ 
			return Unit;
			//InUnit = Unit;
			//UnitFound = true;
			break;
		}
	}

	return nullptr;
	//return UnitFound;
}

TArray<FIntVector> ATBSGrid::GetNeighbours(FIntVector Coordinates)
{
	TArray<FIntVector> Neighbours;

	for (int X = Coordinates.X - 1; X <= Coordinates.X + 1; X++)
	{
		for (int Y = Coordinates.Y - 1; Y <= Coordinates.Y + 1; Y++)
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
	ATBSProp** MatchingProp = Props.FindByPredicate([Coordinates](const ATBSProp* Prop) {
		if (Prop->GameCoordinates == Coordinates)
		{
			return true;
		}

		return false;
	});

	if (MatchingProp && (*MatchingProp)->BlocksAccess)
	{
		return false;
	}

	return true;
}

void ATBSGrid::ReindexProps_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Reindex Props")));
}

void ATBSGrid::ReindexUnits_Implementation()
{
	if (HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Server Reindex Units")));

		TArray<ATBSUnit*> Team0Units = GetUnitsByTeam(0);
		TArray<ATBSUnit*> Team1Units = GetUnitsByTeam(1);

		for (auto& Unit0 : Team0Units)
		{
			for (auto& Unit1 : Team1Units)
			{
				if (CanDrawLineOfFire(Unit0->GameCoordinates, Unit1->GameCoordinates))
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Can draw line of fire (%i, %i, %i) -> (%i, %i, %i)"), Unit0->GameCoordinates.X, Unit0->GameCoordinates.Y, Unit0->GameCoordinates.Z, Unit1->GameCoordinates.X, Unit1->GameCoordinates.Y, Unit1->GameCoordinates.Z));

					Unit0->SeenByTeams.Empty();
					Unit0->SeenByTeams.Add(1);
					Unit0->NetPriority = 3;

					Unit1->SeenByTeams.Empty();
					Unit1->SeenByTeams.Add(0);
				}
				else
				{
					OnActorNoLongerVisible.Broadcast(0, Unit1);
					OnActorNoLongerVisible.Broadcast(1, Unit0);
					Unit0->SeenByTeams.Empty();
					Unit1->SeenByTeams.Empty();
				}
			}
		}

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Reindex Units")));
	}	
}

bool ATBSGrid::CanDrawLineOfFire(FIntVector Start, FIntVector End)
{
	for (auto& Coordinate : Trace(Start, End))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Tracing (%i, %i, %i)"), Coordinate.X, Coordinate.Y, Coordinate.Z));

		if (!IsAccessible(Coordinate))
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Is not accessible")));
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

TArray<ATBSUnit*> ATBSGrid::GetUnitsByTeam(int32 TeamNumber)
{
	return Units.FilterByPredicate([&](const ATBSUnit* Unit) {
		return Unit->TeamNumber == TeamNumber;
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
