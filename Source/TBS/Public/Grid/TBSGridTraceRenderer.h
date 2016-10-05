// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSGrid.h"
#include "TBSGridUI.h"
#include "TBSGridTraceRenderer.generated.h"

UCLASS()
class TBS_API ATBSGridTraceRenderer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBSGridTraceRenderer();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Initialise(ATBSGrid* InGrid, ATBSGridUI* InGridUI);
	void RenderTrace(FIntVector Start, FIntVector End, TArray<FIntVector> Path);
	void ClearTraces();

private:
	UStaticMeshComponent* CreateStepComponent();

	UStaticMesh* StepComponentMesh;
	UMaterial* StepComponentMaterial;
	ATBSGrid* Grid;
	ATBSGridUI* GridUI;
	TArray<UStaticMeshComponent*> RenderedTraceSteps;
	
};
