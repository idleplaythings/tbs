// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSUnit.h"
#include "TBSUnitFactory.generated.h"

UCLASS()
class TBS_API ATBSUnitFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBSUnitFactory();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	ATBSUnit* CreateUnit(FIntVector Coordinates, FRotator Rotation);

private:
	UClass* UnitBPClass;
};
