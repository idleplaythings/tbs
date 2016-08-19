// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "TBSTypes.h"
#include "TBSUnit.generated.h"

UCLASS(Blueprintable)
class TBS_API ATBSUnit : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATBSUnit();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(Replicated, BlueprintReadWrite)
	float Speed = 0.0;

	UPROPERTY(Replicated)
	float AccelerationTime = 1.0;

	UPROPERTY(Replicated)
	float TurnTime = 0.5;

	UPROPERTY(Replicated)
	float Acceleration = 0.5;

	UPROPERTY(Replicated)
	float TurningSpeed = 10;

	UPROPERTY(Replicated)
	float MaxSpeed = 2;

	UPROPERTY(Replicated)
	FIntVector GameCoordinates;

	void AddMovementCommand();
};
