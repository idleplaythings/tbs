// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSLevelBuilder.h"


// Sets default values
ATBSLevelBuilder::ATBSLevelBuilder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATBSLevelBuilder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBSLevelBuilder::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATBSLevelBuilder::BuildLevel(const ATBSGrid* Grid)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Building level")));
}
