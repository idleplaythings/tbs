// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSGrid.h"
#include "TBSGridUI.h"
#include "TBSUnitManager.generated.h"

UCLASS()
class TBS_API ATBSUnitManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBSUnitManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Initialise(ATBSGrid* Grid, ATBSGridUI* GridUI);
	void RenderUnits();

private:
	ATBSGrid* Grid;
	ATBSGridUI* GridUI;
};
