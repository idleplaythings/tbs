// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSTerrainWall01.h"


// Sets default values for this component's properties
UTBSTerrainWall01::UTBSTerrainWall01()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> IntactMesh(TEXT("/Game/Meshes/tileIntact.tileIntact"));

	Bottom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bottom"));
	Bottom->SetStaticMesh(IntactMesh.Object);
	Bottom->AddRelativeRotation(FQuat(FVector(1.0, 0.0, 0.0), PI/2));
	Bottom->AddRelativeLocation(FVector(0.0, 0.0, 100.0));
	Bottom->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

	Middle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Middle"));
	Middle->SetStaticMesh(IntactMesh.Object);
	Middle->AddRelativeRotation(FQuat(FVector(1.0, 0.0, 0.0), PI / 2));
	Middle->AddRelativeLocation(FVector(0.0, 0.0, 300.0));
	Middle->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

	Top = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Top"));
	Top->SetStaticMesh(IntactMesh.Object);
	Top->AddRelativeRotation(FQuat(FVector(1.0, 0.0, 0.0), PI / 2));
	Top->AddRelativeLocation(FVector(0.0, 0.0, 500.0));
	Top->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
}


// Called when the game starts
void UTBSTerrainWall01::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTBSTerrainWall01::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

