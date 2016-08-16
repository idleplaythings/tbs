// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSGridCursor.h"


// Sets default values
ATBSGridCursor::ATBSGridCursor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATBSGridCursor::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ATBSGridCursor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	TimeSinceLastCursorUpdate += DeltaTime;

	if (TimeSinceLastCursorUpdate < CursorUpdateFrequency)
	{
		return;
	}
/*
	TimeSinceLastCursorUpdate = 0;

	FHitResult Result;	

	ATBSPlayerController* Controller = Cast<ATBSPlayerController>(GetWorld()->GetFirstPlayerController());

	if (Controller != nullptr)
	{
		bool bHitSomething = Controller->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, true, Result);

		if (bHitSomething)
		{
			SetActorLocation(FVector(Result.Location.X, Result.Location.Y, Result.Location.Z));
		}
	}*/
}
