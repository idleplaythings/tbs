// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSGrid.h"
#include "TBSGridUI.h"
#include "TBSGridPathRenderer.generated.h"

UCLASS()
class TBS_API ATBSGridPathRenderer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBSGridPathRenderer();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Initialise(ATBSGrid* InGrid, ATBSGridUI* InGridUI);
	void RenderPath(TArray<FIntVector> Path);
	void ClearPath();

private:
	UStaticMeshComponent* CreatePathComponent();

	UStaticMesh* PathComponentMesh;
	UMaterial* PathComponentMaterial;
	ATBSGrid* Grid;
	ATBSGridUI* GridUI;
	TArray<UStaticMeshComponent*> RenderedPath;
};
