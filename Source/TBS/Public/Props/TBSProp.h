// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSProp.generated.h"

UCLASS()
class TBS_API ATBSProp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBSProp();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Prop")
	FIntVector Dimensions = FIntVector(1, 1, 1);

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Prop")
	FIntVector GameCoordinates;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Prop")
	TArray<FIntVector> GameCoordinatesOccupied;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Prop")
	FRotator Rotation;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Prop")
	bool BlocksAccess;

	void RecalculateCoordinates();

	virtual void ATBSProp::ScalePropMesh();
};
