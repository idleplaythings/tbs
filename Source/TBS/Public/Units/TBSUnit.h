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

	UPROPERTY(BlueprintReadWrite)
	float Speed = 0.0;

	float AccelerationTime = 1.0;
	float TurnTime = 0.5;

	float Acceleration = 0.5;

	float TurningSpeed = 10;

	float MaxSpeed = 2;
};
