// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "UnrealNetwork.h"
#include "TBSProp.h"


// Sets default values
ATBSProp::ATBSProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ATBSProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBSProp::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATBSProp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATBSProp, BlocksAccess);
	DOREPLIFETIME(ATBSProp, Rotation);
	DOREPLIFETIME(ATBSProp, GameCoordinates);
}
