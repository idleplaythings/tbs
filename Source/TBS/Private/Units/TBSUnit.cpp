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
	//RecalculateCoordinates();

	if (Material)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
		SkeletalMesh->SetMaterial(0, DynamicMaterial);

		if (PlayerNumber == 0)
		{
			if (Dimensions.X == 1)
			{
				DynamicMaterial->SetVectorParameterValue(FName(TEXT("TeamColor")), FLinearColor(0.9, 0.9, 0.4));
			}
			else if (Dimensions.X == 2)
			{
				DynamicMaterial->SetVectorParameterValue(FName(TEXT("TeamColor")), FLinearColor(0.9, 0.9, 0.2));
			}
			else
			{
				DynamicMaterial->SetVectorParameterValue(FName(TEXT("TeamColor")), FLinearColor(0.9, 0.9, 0.1));
			}
		}
		else
		{
			if (Dimensions.X == 1)
			{
				DynamicMaterial->SetVectorParameterValue(FName(TEXT("TeamColor")), FLinearColor(0.9, 0.4, 0.9));
			}
			else if (Dimensions.X == 2)
			{
				DynamicMaterial->SetVectorParameterValue(FName(TEXT("TeamColor")), FLinearColor(0.9, 0.2, 0.9));
			}
			else
			{
				DynamicMaterial->SetVectorParameterValue(FName(TEXT("TeamColor")), FLinearColor(0.9, 0.1, 0.9));
			}
		}
	}

	ScaleUnitMesh();
}

// Called every frame
void ATBSUnit::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ATBSUnit::RecalculateCoordinates()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Recalculating coordinates, dimensions (%i, %i, %i)"), Dimensions.X, Dimensions.Y, Dimensions.Z));

	TArray<FIntVector> NewCoordinates;

	int32 XStep = Dimensions.X % 2 == 0 ? -5 : 0;
	int32 YStep = Dimensions.Y % 2 == 0 ? -5 : 0;

	FIntVector Origin = FIntVector(
		GameCoordinates.X + XStep - (FMath::CeilToInt((float)Dimensions.X / 2) * 10 - 10),
		GameCoordinates.Y + YStep - (FMath::CeilToInt((float)Dimensions.Y / 2) * 10 - 10),
		GameCoordinates.Z
	);

	for (int32 X = 0; X < Dimensions.X * 10; X = X + 10)
	{
		for (int32 Y = 0; Y < Dimensions.Y * 10; Y = Y + 10)
		{
			for (int32 Z = 0; Z < Dimensions.Z * 10; Z = Z + 10)
			{
				NewCoordinates.Add(FIntVector(
					Origin.X + X,
					Origin.Y + Y,
					Origin.Z + Z
				));
			}
		}
	}

	GameCoordinatesOccupied = NewCoordinates;
}

void ATBSUnit::MoveNorth()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("North")));
	TArray<FIntVector> NewCoordinates;

	for (auto& Coordinates : GameCoordinatesOccupied)
	{
		NewCoordinates.Add(FIntVector(Coordinates.X, Coordinates.Y - 10, Coordinates.Z));
	}

	GameCoordinates.Y -= 10;

	GameCoordinatesOccupied = NewCoordinates;
}

void ATBSUnit::MoveEast()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("East")));
	TArray<FIntVector> NewCoordinates;

	for (auto& Coordinates : GameCoordinatesOccupied)
	{
		NewCoordinates.Add(FIntVector(Coordinates.X + 10, Coordinates.Y, Coordinates.Z));
	}

	GameCoordinates.X += 10;

	GameCoordinatesOccupied = NewCoordinates;
}

void ATBSUnit::MoveSouth()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("South")));
	TArray<FIntVector> NewCoordinates;

	for (auto& Coordinates : GameCoordinatesOccupied)
	{
		NewCoordinates.Add(FIntVector(Coordinates.X, Coordinates.Y + 10, Coordinates.Z));
	}

	GameCoordinates.Y += 10;

	GameCoordinatesOccupied = NewCoordinates;
}

void ATBSUnit::MoveWest()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("West")));
	TArray<FIntVector> NewCoordinates;

	for (auto& Coordinates : GameCoordinatesOccupied)
	{
		NewCoordinates.Add(FIntVector(Coordinates.X - 10, Coordinates.Y, Coordinates.Z));
	}

	GameCoordinates.X -= 10;

	GameCoordinatesOccupied = NewCoordinates;
}

void ATBSUnit::ScaleUnitMesh()
{
	float Scale = (float) Dimensions.X / 2;
	SkeletalMesh->SetRelativeScale3D(FVector(Scale, Scale, Scale));
}
