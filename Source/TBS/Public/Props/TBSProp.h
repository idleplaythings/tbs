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

	UPROPERTY(BlueprintReadOnly, Category = "Prop")
	FIntVector Dimensions = FIntVector(1, 1, 1);

	UPROPERTY(BlueprintReadOnly, Category = "Prop")
	FIntVector GameCoordinates;

	UPROPERTY(BlueprintReadOnly, Category = "Prop")
	TArray<FIntVector> GameCoordinatesOccupied;

	UPROPERTY(BlueprintReadOnly, Category = "Prop")
	FRotator Rotation;

	UPROPERTY(BlueprintReadOnly, Category = "Prop")
	bool BlocksAccess;

	void RecalculateCoordinates();

	virtual void ATBSProp::ScalePropMesh();
};
