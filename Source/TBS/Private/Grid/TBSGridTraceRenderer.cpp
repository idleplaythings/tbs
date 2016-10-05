// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSGridTraceRenderer.h"


// Sets default values
ATBSGridTraceRenderer::ATBSGridTraceRenderer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	StepComponentMesh = Mesh.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Materials/M_GridPathmat.M_GridPathmat'"));
	StepComponentMaterial = Material.Object;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

// Called when the game starts or when spawned
void ATBSGridTraceRenderer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBSGridTraceRenderer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATBSGridTraceRenderer::Initialise(ATBSGrid* InGrid, ATBSGridUI* InGridUI)
{
	Grid = InGrid;
	GridUI = InGridUI;
}

void ATBSGridTraceRenderer::RenderTrace(FIntVector Start, FIntVector End, TArray<FIntVector> InPath)
{
	if (RenderedTraceSteps.Num() < InPath.Num())
	{
		int ComponentsToAdd = InPath.Num() - RenderedTraceSteps.Num();
		for (int i = 0; i < ComponentsToAdd; i++)
		{
			RenderedTraceSteps.Add(CreateStepComponent());
		}
	}

	for (int i = 0; i < InPath.Num(); i++)
	{
		FCoordinateLocations Locations = GridUI->GetCoordinateLocations(InPath[i]);
		RenderedTraceSteps[i]->SetRelativeLocation(Locations.Center);
		RenderedTraceSteps[i]->SetVisibility(true);
	}
}

void ATBSGridTraceRenderer::ClearTraces()
{
	for (auto& TraceStep : RenderedTraceSteps)
	{
		TraceStep->SetVisibility(false);
	}
}

UStaticMeshComponent* ATBSGridTraceRenderer::CreateStepComponent()
{
	UStaticMeshComponent* StepComponent = NewObject<UStaticMeshComponent>(this);
	StepComponent->SetStaticMesh(StepComponentMesh);
	StepComponent->SetMaterial(0, StepComponentMaterial);
	StepComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	StepComponent->SetRelativeScale3D(FVector(0.3, 0.3, 0.01));
	FinishAndRegisterComponent(StepComponent);
	return StepComponent;
}
