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

	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Unit")
	int32 TeamNumber = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Unit")
	TArray<int32> SeenByTeams;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Unit")
	float Speed = 0.0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Unit")
	float AccelerationTime = 1.0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Unit")
	float TurnTime = 0.5;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Unit")
	float Acceleration = 0.5;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Unit")
	float TurningSpeed = 10;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Unit")
	float MaxSpeed = 2;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Unit")
	FIntVector GameCoordinates;

	void AddMovementCommand();
};
