// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSProp_Block.h"


// Sets default values
ATBSProp_Block::ATBSProp_Block()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("StaticMesh'/Game/Meshes/block.block'"));

	if (Mesh.Succeeded())
	{
		//BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Block"));
		//BlockMesh->SetSimulatePhysics(false);
		//BlockMesh->SetStaticMesh(Mesh.Object);
		//BlockMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

		ISMComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISMC"));
		ISMComponent->SetStaticMesh(Mesh.Object);
		ISMComponent->SetFlags(RF_Transactional);
		AddInstanceComponent(ISMComponent);
	}	
}

// Called when the game starts or when spawned
void ATBSProp_Block::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBSProp_Block::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATBSProp_Block::ScalePropMesh()
{
	if (BlockMesh)
	{
		BlockMesh->SetRelativeScale3D(FVector((float)Dimensions.X / 2, (float)Dimensions.Y / 2, (float)Dimensions.Z / 2));
	}	
}

void ATBSProp_Block::SpawnInstance(const FTransform& InstanceTransform)
{
	if (ISMComponent)
	{
		ISMComponent->AddInstance(InstanceTransform);
	}
}

