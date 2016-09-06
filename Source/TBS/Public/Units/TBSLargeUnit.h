// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSUnit.h"
#include "TBSLargeUnit.generated.h"

UCLASS()
class TBS_API ATBSLargeUnit : public ATBSUnit
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBSLargeUnit();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	FIntVector Dimensions = FIntVector(3, 3, 6);
	
};
