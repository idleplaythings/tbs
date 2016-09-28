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
struct FMapMeta
{
	GENERATED_USTRUCT_BODY()
};

USTRUCT()
struct FProp
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	uint32 Id;

	UPROPERTY()
	uint32 TypeId;

	UPROPERTY()
	FIntVector Coordinates;

	UPROPERTY()
	FIntVector Dimensions;

	UPROPERTY()
	int32 Rotation;

	UPROPERTY()
	FVector Offset;

	UPROPERTY()
	bool BlocksAccess;

	FProp()
	{
		Id = 0;
		TypeId = 0;
		Coordinates = FIntVector(0, 0, 0);
		Dimensions = FIntVector(1, 1, 1);
		Rotation = 0;
		Offset = FVector(0.0, 0.0, 0.0);
		BlocksAccess = false;
	}
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

struct FNetworkMessage
{
	uint32 ConnectionId;
	uint32 Length;
	uint8_t* Data;

	FNetworkMessage()
	{
		ConnectionId = 0;
		Length = 0;
		Data = nullptr;
	}

	FNetworkMessage(uint32 InLength, uint8_t* InData)
	{
		ConnectionId = 0;
		Length = InLength;
		Data = InData;
	}

	FNetworkMessage(uint32 InConnectionId, uint32 InLength, uint8_t* InData)
	{
		ConnectionId = InConnectionId;
		Length = InLength;
		Data = InData;
	}
};

