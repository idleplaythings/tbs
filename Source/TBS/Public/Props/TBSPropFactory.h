// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSPropFactory.generated.h"

UCLASS()
class TBS_API ATBSPropFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBSPropFactory();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	ATBSProp* CreateWall(FIntVector Coordinates, FRotator Rotation);
};
