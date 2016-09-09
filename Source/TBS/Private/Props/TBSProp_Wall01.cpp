// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSProp_Wall01.h"


// Sets default values
ATBSProp_Wall01::ATBSProp_Wall01()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> IntactMesh(TEXT("/Game/Meshes/tileIntact.tileIntact"));

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;

	Bottom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bottom"));
	Bottom->SetStaticMesh(IntactMesh.Object);
	Bottom->AddRelativeRotation(FQuat(FVector(1.0, 0.0, 0.0), PI / 2));
	Bottom->AddRelativeLocation(FVector(0.0, 0.0, 50.0));
	Bottom->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	Middle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Middle"));
	Middle->SetStaticMesh(IntactMesh.Object);
	Middle->AddRelativeRotation(FQuat(FVector(1.0, 0.0, 0.0), PI / 2));
	Middle->AddRelativeLocation(FVector(0.0, 0.0, 100.0));
	Middle->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	Top = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Top"));
	Top->SetStaticMesh(IntactMesh.Object);
	Top->AddRelativeRotation(FQuat(FVector(1.0, 0.0, 0.0), PI / 2));
	Top->AddRelativeLocation(FVector(0.0, 0.0, 150.0));
	Top->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
}

// Called when the game starts or when spawned
void ATBSProp_Wall01::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBSProp_Wall01::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

