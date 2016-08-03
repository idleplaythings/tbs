// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSProp.h"
#include "TBSGrid.generated.h"

UCLASS()
class TBS_API ATBSGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBSGrid();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
	int32 GridWidth = 0;
	int32 GridHeight = 0;
	int32 NumOfLevels = 0;

	void InitialiseGrid(int32 GridWidth, int32 GridHeight, int32 NumOfLevels);
	void AddProp(FProp* Prop);
	void AddUnit(FUnit* Unit);
	TMap<FString, TArray<FProp*>>::TConstIterator GetPropsIterator();
	TArray<FUnit*>::TConstIterator GetUnitIterator();

private:
	TMap<FString, TArray<FProp*>> Props;
	TArray<FUnit*> Units;
};
