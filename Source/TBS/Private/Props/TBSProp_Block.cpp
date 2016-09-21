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

void ATBSProp_Block::SpawnInstance(FIntVector Coordinates, const FTransform& InstanceTransform)
{
	if (ISMComponent)
	{
		int32 InstanceIndex = ISMComponent->AddInstanceWorldSpace(InstanceTransform);
		InstanceMap.Add(Coordinates, InstanceIndex);

		if (Debug)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Added index %i"), InstanceIndex));
		}
	}
}

void ATBSProp_Block::RemoveInstance(FIntVector Coordinates)
{
	if (ISMComponent)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Looking for instance index (%i, %i, %i)"), Coordinates.X, Coordinates.Y, Coordinates.Z));
		int32* InstanceIndex = InstanceMap.Find(Coordinates);

		if (InstanceIndex)
		{				
			ISMComponent->UpdateInstanceTransform(*InstanceIndex, FTransform(FVector(10000.0, 10000.0, 10000.0)), true, true);
			RemovedIndexes.Add(*InstanceIndex);
			//if (ISMComponent->RemoveInstance(*InstanceIndex))
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Removed index %i"), *InstanceIndex));
			//	InstanceMap.Remove(Coordinates);

			//	for (auto& It : InstanceMap)
			//	{
			//		if (It.Value > *InstanceIndex)
			//		{
			//			InstanceMap[It.Key] = It.Value - 1;
			//		}
			//	}
			//}			
		}
	}
}

void ATBSProp_Block::PurgeRemoved()
{

}
