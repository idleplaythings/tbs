// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSGridPathRenderer.h"


// Sets default values
ATBSGridPathRenderer::ATBSGridPathRenderer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	PathComponentMesh = Mesh.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Materials/M_GridPathmat.M_GridPathmat'"));
	PathComponentMaterial = Material.Object;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

// Called when the game starts or when spawned
void ATBSGridPathRenderer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBSGridPathRenderer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATBSGridPathRenderer::Initialise(ATBSGrid* InGrid, ATBSGridUI* InGridUI)
{
	Grid = InGrid;
	GridUI = InGridUI;
}

void ATBSGridPathRenderer::RenderPath(TArray<FIntVector> InPath)
{
	//return; 

	if (RenderedPath.Num() < InPath.Num())
	{
		int ComponentsToAdd = InPath.Num() - RenderedPath.Num();
		for (int i = 0; i < ComponentsToAdd; i++)
		{
			RenderedPath.Add(CreatePathComponent());
		}
	}

	for (int i = 0; i < InPath.Num(); i++)
	{
		FCoordinateLocations Locations = GridUI->GetCoordinateLocations(InPath[i]);
		RenderedPath[i]->SetRelativeLocation(Locations.Center);
		RenderedPath[i]->SetVisibility(true);
	}
}

void ATBSGridPathRenderer::ClearPath()
{
	//return;

	for (auto& PathComponent : RenderedPath)
	{
		PathComponent->SetVisibility(false);
	}
}

UStaticMeshComponent* ATBSGridPathRenderer::CreatePathComponent()
{
	UStaticMeshComponent* PathComponent = NewObject<UStaticMeshComponent>(this);
	PathComponent->SetStaticMesh(PathComponentMesh);
	PathComponent->SetMaterial(0, PathComponentMaterial);
	PathComponent->AttachTo(RootComponent);
	PathComponent->SetRelativeScale3D(FVector(0.3, 0.3, 0.01));
	FinishAndRegisterComponent(PathComponent);
	return PathComponent;

}
