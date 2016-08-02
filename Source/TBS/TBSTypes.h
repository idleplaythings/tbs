// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TBSTypes.generated.h"

class ATBSProp;

UENUM(BlueprintType)
enum class ETileSlot : uint8
{
	Center UMETA(DisplayName = "Center"),
	NW UMETA(DisplayName = "Northwest"),
	N UMETA(DisplayName = "North"),
	NE UMETA(DisplayName = "Northeast"),
	E UMETA(DisplayName = "East"),
	SE UMETA(DisplayName = "SouthEast"),
	S UMETA(DisplayName = "South"),
	SW UMETA(DisplayName = "Southwest"),
	W UMETA(DisplayName = "West")
};

struct FProp
{
	FGuid* Guid;
	UClass* PropClass;
	ATBSProp* Prop;
	FIntVector Coordinates;
	ETileSlot Slot;
	FRotator Rotation;

	FProp()
	{
		Guid = new FGuid();
	}
};

struct FCoordinateLocations
{
	FVector Center;
	FVector NW;
	FVector N;
	FVector NE;
	FVector E;
	FVector SE;
	FVector S;
	FVector SW;
	FVector W;
};
