// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "TBSTerrainWall01.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TBS_API UTBSTerrainWall01 : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTBSTerrainWall01();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UStaticMesh* IntactMesh;

	UStaticMeshComponent* Bottom;
	UStaticMeshComponent* Middle;
	UStaticMeshComponent* Top;
};
