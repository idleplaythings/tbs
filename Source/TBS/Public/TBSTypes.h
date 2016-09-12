// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TBSTypes.generated.h"

class ATBSProp;
class ATBSUnit;

USTRUCT()
struct FCommand
{
	GENERATED_BODY()
};

USTRUCT()
struct FMovementCommand
{
	GENERATED_BODY()

	UPROPERTY()
	ATBSUnit* Unit;

	UPROPERTY()
	TArray<FIntVector> Movements;
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

USTRUCT()
struct FProp
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 Id;

	UPROPERTY()
	int32 Rotation;

	UPROPERTY()
	FIntVector Coordinates;
};

FORCEINLINE FArchive& operator<<(FArchive &Ar, FProp& Prop)
{
	Ar << Prop.Id;
	Ar << Prop.Coordinates;

	return Ar;
}

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

