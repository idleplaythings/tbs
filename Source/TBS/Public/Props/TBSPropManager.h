// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSProp.h"
#include "TBSGrid.h"
#include "TBSGridUI.h"
#include "TBSPropManager.generated.h"

UCLASS()
class TBS_API ATBSPropManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBSPropManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Initialise(ATBSGrid* Grid, ATBSGridUI* GridUI);
	void RenderProps();

private:
	ATBSGrid* Grid;
	ATBSGridUI* GridUI;
};
