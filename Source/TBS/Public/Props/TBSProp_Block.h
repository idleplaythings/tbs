// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSProp.h"
#include "TBSProp_Block.generated.h"

UCLASS()
class TBS_API ATBSProp_Block : public ATBSProp
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBSProp_Block();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	USceneComponent* SceneComponent;
	UStaticMeshComponent* BlockMesh;
	
	void ScalePropMesh() override;
};
