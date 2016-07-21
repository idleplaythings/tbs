// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSGrid.h"


// Sets default values
ATBSGrid::ATBSGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Plane(TEXT("/Game/Meshes/plane.plane"));
	static ConstructorHelpers::FObjectFinder<UMaterial> GridMaterial(TEXT("/Game/Materials/GridMaterial.GridMaterial"));

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;

	GridComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grid"));
	GridComponent->SetStaticMesh(Plane.Object);
	GridComponent->SetMaterial(0, GridMaterial.Object);
	GridComponent->AddRelativeLocation(FVector(0.0, 0.0, 1));

	GridComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GridComponent->SetCollisionProfileName(FName(TEXT("GridCollisionProfile")));

}

// Called when the game starts or when spawned
void ATBSGrid::BeginPlay()
{
	Super::BeginPlay();

	GridComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SetActorLocation(FVector(0.0, 0.0, 0.0));
	GridComponent->SetRelativeScale3D(FVector((float)(GridWidth * TileSize), (float)(GridHeight * TileSize), 0.01));

	//SetActorScale3D(FVector((float)(GridWidth * TileSize), (float)(GridHeight * TileSize), 0.01));
}

// Called every frame
void ATBSGrid::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

