// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSGrid.generated.h"

UCLASS(Blueprintable)
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

	USceneComponent* SceneComponent;

	UPROPERTY(Category = Grid, BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* GridComponent;

	UPROPERTY(Category = Grid, BlueprintReadWrite, EditAnywhere)
	int GridWidth = 100;

	UPROPERTY(Category = Grid, BlueprintReadWrite, EditAnywhere)
	int GridHeight = 100;

	UPROPERTY(Category = Grid, BlueprintReadWrite, EditAnywhere)
	int TileSize = 40;
};

