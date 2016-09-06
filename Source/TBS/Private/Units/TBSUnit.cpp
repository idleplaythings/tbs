// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "UnrealNetwork.h"
#include "TBSPlayerController.h"
#include "TBSUnit.h"


// Sets default values
ATBSUnit::ATBSUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeleton"));
	SkeletalMesh->AddRelativeRotation(FRotator(0.0, -90.0, 0.0));
	SkeletalMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> UnitAnimation(TEXT("AnimBlueprint'/Game/Animations/BP_UnitAnim.BP_UnitAnim'"));
	if (UnitAnimation.Succeeded())
	{
		SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		SkeletalMesh->SetAnimInstanceClass(UnitAnimation.Object->GeneratedClass);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> UnitSkeleton(TEXT("SkeletalMesh'/Game/Characters/character1fbx.character1fbx'"));
	if (UnitSkeleton.Succeeded())
	{
		SkeletalMesh->SetSkeletalMesh(UnitSkeleton.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> UnitMaterial(TEXT("Material'/Game/Materials/M_UnitMaterial.M_UnitMaterial'"));
	if (UnitMaterial.Succeeded())
	{
		Material = UnitMaterial.Object;
	}
}

bool ATBSUnit::IsNetRelevantFor(const AActor * RealViewer, const AActor * ViewTarget, const FVector & SrcLocation) const
{
	if (Cast<ATBSPlayerController>(RealViewer)->PlayerNumber == PlayerNumber)
	{
		return true;
	}

	return SeenByPlayers.Find(Cast<ATBSPlayerController>(RealViewer)->PlayerNumber) != INDEX_NONE;
}


void ATBSUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATBSUnit, PlayerNumber);
	DOREPLIFETIME(ATBSUnit, SeenByPlayers);
	DOREPLIFETIME(ATBSUnit, Speed);
	DOREPLIFETIME(ATBSUnit, AccelerationTime);
	DOREPLIFETIME(ATBSUnit, TurnTime);
	DOREPLIFETIME(ATBSUnit, Acceleration);
	DOREPLIFETIME(ATBSUnit, TurningSpeed);
	DOREPLIFETIME(ATBSUnit, MaxSpeed);
	DOREPLIFETIME(ATBSUnit, GameCoordinatesOccupied);
	DOREPLIFETIME(ATBSUnit, GameCoordinates);
	DOREPLIFETIME(ATBSUnit, Dimensions);
}

// Called when the game starts or when spawned
void ATBSUnit::BeginPlay()
{
	Super::BeginPlay();

	if (Material)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
		SkeletalMesh->SetMaterial(0, DynamicMaterial);

		if (PlayerNumber == 0)
		{
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("TeamColor")), FLinearColor(0.915, 0.674, 0.0));
		}
		else
		{
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("TeamColor")), FLinearColor(0.865, 0.0, 0.13));
		}
	}
}

// Called every frame
void ATBSUnit::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ATBSUnit::RecalculateCoordinates()
{
	TArray<FIntVector> NewCoordinates;

	if (Dimensions.X % 2 == 0)
	{
		int32 StepsX = Dimensions.X / 2 - 1;
		int32 OffsetX = StepsX * 10;
		int32 StepsY = Dimensions.Y / 2 - 1;
		int32 OffsetY = StepsY * 10;


		for (int32 X = (GameCoordinates.X - 5) - OffsetX; X <= (GameCoordinates.X + 5) - OffsetX; X += 10)
		{
			for (int32 Y = (GameCoordinates.Y - 5) - OffsetY; Y <= (GameCoordinates.Y + 5) - OffsetY; Y += 10)
			{
				for (int32 Z = GameCoordinates.Z; Z <= Dimensions.Z; Z++)
				{
					NewCoordinates.Add(FIntVector(X, Y, Z));
				}
			}
		}
	}
	else
	{

	}

	GameCoordinatesOccupied = NewCoordinates;
}

void ATBSUnit::MoveNorth()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("North")));
	TArray<FIntVector> NewCoordinates;

	for (auto& Coordinates : GameCoordinatesOccupied)
	{
		NewCoordinates.Add(FIntVector(Coordinates.X, Coordinates.Y - 10, Coordinates.Z));
	}

	GameCoordinatesOccupied = NewCoordinates;
}

void ATBSUnit::MoveEast()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("East")));
	TArray<FIntVector> NewCoordinates;

	for (auto& Coordinates : GameCoordinatesOccupied)
	{
		NewCoordinates.Add(FIntVector(Coordinates.X + 10, Coordinates.Y, Coordinates.Z));
	}

	GameCoordinatesOccupied = NewCoordinates;
}

void ATBSUnit::MoveSouth()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("South")));
	TArray<FIntVector> NewCoordinates;

	for (auto& Coordinates : GameCoordinatesOccupied)
	{
		NewCoordinates.Add(FIntVector(Coordinates.X, Coordinates.Y + 10, Coordinates.Z));
	}

	GameCoordinatesOccupied = NewCoordinates;
}

void ATBSUnit::MoveWest()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("West")));
	TArray<FIntVector> NewCoordinates;

	for (auto& Coordinates : GameCoordinatesOccupied)
	{
		NewCoordinates.Add(FIntVector(Coordinates.X - 10, Coordinates.Y, Coordinates.Z));
	}

	GameCoordinatesOccupied = NewCoordinates;
}
