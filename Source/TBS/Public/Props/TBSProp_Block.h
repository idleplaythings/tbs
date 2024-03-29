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

	UStaticMesh* BlockMesh;

	USceneComponent* SceneComponent;
	UStaticMeshComponent* SMC;
	UInstancedStaticMeshComponent* ISMC;
	
	void ScalePropMesh() override;
	void SpawnInstance(uint32 PropId, const FTransform& InstanceTransform);
	void RemoveInstance(uint32 PropId);

	TMap<uint32, int32> InstanceMap;

	bool Debug = false;
};
