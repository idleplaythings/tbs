// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSGrid.h"


// Sets default values
ATBSGrid::ATBSGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATBSGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBSGrid::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATBSGrid::InitialiseGrid(int32 GridWidth, int32 GridHeight, int32 NumOfLevels)
{
	this->GridWidth = GridWidth;
	this->GridHeight = GridHeight;
	this->NumOfLevels = NumOfLevels;
}

TMap<FIntVector, TArray<FProp>>::TConstIterator ATBSGrid::GetPropsIterator()
{
	return Props.CreateConstIterator();
}

TArray<FUnit*>::TIterator ATBSGrid::GetUnitIterator()
{
	return Units.CreateIterator();
}

void ATBSGrid::AddProp(FProp Prop)
{
	TArray<FProp>* Result = Props.Find(Prop.Coordinates);

	if (Result != nullptr)
	{
		Result->Add(Prop);
	}
	else
	{
		TArray<FProp> Arr;
		Arr.Add(Prop);
		Props.Add(Prop.Coordinates, Arr);
	}
}

void ATBSGrid::AddUnit(FUnit* Unit)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Add unit addr (%#010x)"), Unit));
	Units.Add(Unit);
}

FUnit* ATBSGrid::SelectUnit(FIntVector GameCoords)
{
	//bool UnitFound = false;
	for (auto &Unit : Units)
	{
		if (Unit->Coordinates == GameCoords)
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
			&& Neighbour.X >= 0 && Neighbour.X < this->GridWidth
			&& Neighbour.Y >= 0 && Neighbour.Y < this->GridHeight
			&& Neighbour.Z >= 0 && Neighbour.Z < this->NumOfLevels;
	});
}

TArray<FIntVector> ATBSGrid::GetAccessibleNeighbours(FIntVector Coordinates)
{
	TArray<FIntVector> Neighbours = GetNeighbours(Coordinates);

	return Neighbours.FilterByPredicate([this, Coordinates](const FIntVector& Neighbour) {
		bool SameX = Coordinates.X == Neighbour.X;
		bool SameY = Coordinates.Y == Neighbour.Y;

		// If only either X or Y coordinates change, neighbour is directly adjacent
		// and we only need to consider props blocking direct access
		if (SameX ? !SameY : SameY)
		{
			if (SameX)
			{
				if (Neighbour.Y > Coordinates.Y)
				{					
					return !IsTileAccessBlockedByProp(Coordinates, ETileSlot::S)
						&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::N);
				}
				else
				{	
					return !IsTileAccessBlockedByProp(Coordinates, ETileSlot::N)
						&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::S);
				}
			}
			else {
				if (Neighbour.X > Coordinates.X)
				{
					return !IsTileAccessBlockedByProp(Coordinates, ETileSlot::E)
						&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::W);
				}
				else
				{
					return !IsTileAccessBlockedByProp(Coordinates, ETileSlot::W)
						&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::E);
				}
			}
		}
		// Otherwise neighbour is diagonally adjacent, and we need to consider blocking access
		// in neighbouring tiles as well
		else
		{
			if (Neighbour.X > Coordinates.X)
			{
				if (Neighbour.Y > Coordinates.Y)
				{
					FIntVector OneEast = Coordinates + FIntVector(1, 0, 0);
					FIntVector OneSouth = Coordinates + FIntVector(0, 1, 0);

					return !IsTileAccessBlockedByProp(Coordinates, ETileSlot::E)
						&& !IsTileAccessBlockedByProp(Coordinates, ETileSlot::S)
						&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::N)
						&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::W)
						&& !IsTileAccessBlockedByProp(OneEast, ETileSlot::W)
						&& !IsTileAccessBlockedByProp(OneEast, ETileSlot::S)
						&& !IsTileAccessBlockedByProp(OneSouth, ETileSlot::N)
						&& !IsTileAccessBlockedByProp(OneSouth, ETileSlot::E);
				}
				else {
					FIntVector OneEast = Coordinates + FIntVector(1, 0, 0);
					FIntVector OneNorth = Coordinates + FIntVector(0, -1, 0);

					return !IsTileAccessBlockedByProp(Coordinates, ETileSlot::E)
						&& !IsTileAccessBlockedByProp(Coordinates, ETileSlot::N)
						&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::S)
						&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::W)
						&& !IsTileAccessBlockedByProp(OneEast, ETileSlot::W)
						&& !IsTileAccessBlockedByProp(OneEast, ETileSlot::N)
						&& !IsTileAccessBlockedByProp(OneNorth, ETileSlot::S)
						&& !IsTileAccessBlockedByProp(OneNorth, ETileSlot::E);
				}
			}
			else {
				if (Neighbour.Y > Coordinates.Y)
				{
					FIntVector OneWest = Coordinates + FIntVector(-1, 0, 0);
					FIntVector OneSouth = Coordinates + FIntVector(0, 1, 0);

					return !IsTileAccessBlockedByProp(Coordinates, ETileSlot::W)
						&& !IsTileAccessBlockedByProp(Coordinates, ETileSlot::S)
						&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::N)
						&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::E)
						&& !IsTileAccessBlockedByProp(OneWest, ETileSlot::E)
						&& !IsTileAccessBlockedByProp(OneWest, ETileSlot::S)
						&& !IsTileAccessBlockedByProp(OneSouth, ETileSlot::N)
						&& !IsTileAccessBlockedByProp(OneSouth, ETileSlot::W);
				}
				else
				{
					FIntVector OneWest = Coordinates + FIntVector(-1, 0, 0);
					FIntVector OneNorth = Coordinates + FIntVector(0, -1, 0);					

					return !IsTileAccessBlockedByProp(Coordinates, ETileSlot::N)
						&& !IsTileAccessBlockedByProp(Coordinates, ETileSlot::W)
						&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::S)
						&& !IsTileAccessBlockedByProp(Neighbour, ETileSlot::E)
						&& !IsTileAccessBlockedByProp(OneWest, ETileSlot::N)
						&& !IsTileAccessBlockedByProp(OneWest, ETileSlot::E)
						&& !IsTileAccessBlockedByProp(OneNorth, ETileSlot::W)
						&& !IsTileAccessBlockedByProp(OneNorth, ETileSlot::S);
				}
			}
		}
		return true;
	});

	return Neighbours;
}

bool ATBSGrid::IsTileAccessBlockedByProp(FIntVector Coordinates, ETileSlot Slot)
{
	TArray<FProp>* PropsInTile = Props.Find(Coordinates);

	if (PropsInTile == nullptr)
	{
		return false;
	}

	FString Name;
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ETileSlot"), true);
	if (!EnumPtr)
	{
		Name = FString(TEXT("Nope"));
	}
	else
	{
		Name = EnumPtr->GetDisplayNameText((int8)Slot).ToString();
	}	

	for (auto& Prop : *PropsInTile)
	{

		FString Name2;
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ETileSlot"), true);
		if (!EnumPtr)
		{
			Name2 = FString(TEXT("Nope"));
		}
		else
		{
			Name2 = EnumPtr->GetDisplayNameText((int8)Prop.Slot).ToString();
		}

		if (Prop.Slot == Slot && Prop.BlocksAccess)
		{
			return true;
		}
	}

	return false;
}
