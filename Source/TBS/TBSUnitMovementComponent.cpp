// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSUnitMovementComponent.h"

void UTBSUnitMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FVector InputVector = ConsumeInputVector();
	//Acceleration = InputVector;
	PerformMovement(DeltaTime);
}

void UTBSUnitMovementComponent::PerformMovement(float DeltaTime)
{

}

