// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "TBSUnitMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class TBS_API UTBSUnitMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);
	
private:
	void PerformMovement(float DeltaTime);
};
