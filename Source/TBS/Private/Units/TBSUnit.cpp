// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "UnrealNetwork.h"
#include "TBSPlayerController.h"
#include "TBSUnit.h"


// Sets default values
ATBSUnit::ATBSUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;
}

bool ATBSUnit::IsNetRelevantFor(const AActor * RealViewer, const AActor * ViewTarget, const FVector & SrcLocation) const
{
	if (Cast<ATBSPlayerController>(RealViewer)->PlayerNumber == PlayerNumber)
	{
		return true;
	}

	return SeenByPlayers.Find(Cast<ATBSPlayerController>(RealViewer)->PlayerNumber) != INDEX_NONE;
}


void ATBSUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATBSUnit, PlayerNumber);
	DOREPLIFETIME(ATBSUnit, SeenByPlayers);
	DOREPLIFETIME(ATBSUnit, Speed);
	DOREPLIFETIME(ATBSUnit, AccelerationTime);
	DOREPLIFETIME(ATBSUnit, TurnTime);
	DOREPLIFETIME(ATBSUnit, Acceleration);
	DOREPLIFETIME(ATBSUnit, TurningSpeed);
	DOREPLIFETIME(ATBSUnit, MaxSpeed);
	DOREPLIFETIME(ATBSUnit, GameCoordinates);
}

// Called when the game starts or when spawned
void ATBSUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBSUnit::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATBSUnit::AddMovementCommand()
{
	if (Role == ROLE_Authority)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSUnit::AddMovementCommand ROLE_Authority")));
	}
	else if (Role == ROLE_AutonomousProxy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSUnit::AddMovementCommand ROLE_AutonomousProxy")));
	}
	else if (Role == ROLE_SimulatedProxy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSUnit::AddMovementCommand ROLE_SimulatedProxy")));
	}
	else if (Role == ROLE_None)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSUnit::AddMovementCommand ROLE_None")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSUnit::AddMovementCommand Other")));
	}
}
