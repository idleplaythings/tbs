// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "TBSUnit.generated.h"

UCLASS(Blueprintable)
class TBS_API ATBSUnit : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATBSUnit();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
};
