// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSProp_Wall01.generated.h"

UCLASS()
class TBS_API ATBSProp_Wall01 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBSProp_Wall01();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	USceneComponent* SceneComponent;
	UStaticMesh* IntactMesh;
	UStaticMeshComponent* Bottom;
	UStaticMeshComponent* Middle;
	UStaticMeshComponent* Top;
};
