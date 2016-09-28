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

void ATBSProp_Block::SpawnInstance(uint32 PropId, const FTransform& InstanceTransform)
{
	if (ISMComponent)
	{
		int32 InstanceIndex = ISMComponent->AddInstanceWorldSpace(InstanceTransform);
		InstanceMap.Add(PropId, InstanceIndex);

		if (Debug)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Added index %i"), InstanceIndex));
		}
	}
}

void ATBSProp_Block::RemoveInstance(uint32 PropId)
{
	if (ISMComponent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("looking for instance idx on prop %i"), PropId));

		int32* InstanceIndex = InstanceMap.Find(PropId);

		if (InstanceIndex)
		{				
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("got instance idx %i"), *InstanceIndex));

			ISMComponent->UpdateInstanceTransform(*InstanceIndex, FTransform(FVector(10000.0, 10000.0, 10000.0)), true, true);	
		}
	}
}
