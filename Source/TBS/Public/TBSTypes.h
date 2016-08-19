// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "TBSProp.h"
//#include "TBSUnit.h"
#include "TBSTypes.generated.h"

class ATBSProp;
class ATBSUnit;

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

//struct FUnit
//{
//	FGuid* Guid;
//	UClass* UnitClass;
//	ATBSUnit* UnitObject;
//	FIntVector Coordinates;
//	FRotator Rotation;
//
//	FUnit()
//	{
//		Guid = new FGuid();
//	}
//};

struct FMovement
{
	FIntVector TargetGameCoordinates;
	FVector TargetWorldCoordinates;
};

struct FProp
{
	FGuid* Guid;
	UClass* PropClass;
	ATBSProp* PropObject;
	FIntVector Coordinates;
	ETileSlot Slot;
	FRotator Rotation;
	bool BlocksAccess;

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

// From: https://answers.unrealengine.com/questions/180188/analogue-of-priority-queue.html
struct PathStep
{
	FIntVector Coordinates;
	float Priority;

	explicit PathStep()
	{

	}

	explicit PathStep(FIntVector InCoordinates, float InPriority)
	{
		Coordinates = InCoordinates;
		Priority = InPriority;
	}
};

struct PathStepPredicate
{
	bool operator()(const PathStep& A, const PathStep& B) const
	{
		return A.Priority < B.Priority;
	}
};

