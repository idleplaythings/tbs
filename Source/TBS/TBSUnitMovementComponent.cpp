// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSUnit.h"
#include "TBSUnitMovementComponent.h"

void UTBSUnitMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsProcessingMovement)
	{
		if (Cast<ATBSUnit>(PawnOwner)->ConsumeGridMovement(CurrentMovement))
		{
			IsProcessingMovement = true;
		}
	}

	PerformMovement(DeltaTime);
}

void UTBSUnitMovementComponent::PerformMovement(float DeltaTime)
{
	if (IsProcessingMovement)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Performing movement")));
		PawnOwner->AddActorWorldOffset(CurrentMovement.TargetWorldCoordinates - PawnOwner->GetActorLocation());

		IsProcessingMovement = false;
	}
}
