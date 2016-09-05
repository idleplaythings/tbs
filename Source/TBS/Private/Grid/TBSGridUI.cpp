// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
//#include "TBSGameMode.h"
#include "TBSGridCursor.h"
#include "TBSGridUI.h"

// Sets default values
ATBSGridUI::ATBSGridUI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("StaticMesh'/Game/Meshes/1x1mPlane.1x1mPlane'"));
	GridMesh = Mesh.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Materials/M_GridMat.M_GridMat'"));
	GridMaterial = Material.Object;

	static ConstructorHelpers::FObjectFinder<UBlueprint> GridCursorBlueprint(TEXT("Blueprint'/Game/Blueprints/Grid/BP_TBSGridCursor.BP_TBSGridCursor'"));
	GridCursorClass = (UClass*)GridCursorBlueprint.Object->GeneratedClass;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;	
}

// Called when the game starts or when spawned
void ATBSGridUI::BeginPlay()
{
	Super::BeginPlay();

	//InitialisePlayerController();
	SpawnCursor();
}

void ATBSGridUI::RenderGrid(ATBSGrid* InGrid)
{	
	Grid = InGrid;
	InitialiseParametersFromGrid();
	CreateGridMaterialInstances();
	CreateGridMeshComponents();
	UpdateLevelVisibilities();	
}

void ATBSGridUI::InitialiseParametersFromGrid()
{
	GridMeshWidth = (float)(Grid->GridDimensions.X / 10) * TileSize;
	GridMeshHeight = (float)(Grid->GridDimensions.Y / 10) * TileSize;
}

void ATBSGridUI::CreateGridMaterialInstances()
{
	for (int32 i = 0; i < Grid->GridDimensions.Z; i++)
	{
		UMaterialInstanceDynamic* GridMaterialInstance = CreateMaterialInstance();
		GridMaterials.Push(GridMaterialInstance);
	}
}

void ATBSGridUI::CreateGridMeshComponents()
{
	for (int32 i = 0; i < Grid->GridDimensions.Z; i++)
	{
		UStaticMeshComponent* GridMeshComponent = CreateMeshComponent();
		GridMeshComponent->AddRelativeLocation(FVector(0.0, 0.0, (float)i * FloorHeight));
		GridMeshComponent->SetRelativeScale3D(FVector(GridMeshWidth, GridMeshHeight, 0.01));
		GridMeshComponent->SetMaterial(0, GridMaterials[i]);		
		//GridMeshComponent->SetIsReplicated(true);
		GridMeshes.Push(GridMeshComponent);
	}
}

UMaterialInstanceDynamic* ATBSGridUI::CreateMaterialInstance()
{
	UMaterialInstanceDynamic* GridMaterialInstance = UMaterialInstanceDynamic::Create(GridMaterial, this);
	GridMaterialInstance->SetScalarParameterValue(FName(TEXT("UTiling")), (Grid->GridDimensions.X / 10) + 0.02);
	GridMaterialInstance->SetScalarParameterValue(FName(TEXT("VTiling")), (Grid->GridDimensions.Y / 10) + 0.02);
	GridMaterialInstance->SetScalarParameterValue(FName(TEXT("OpacityMultiplier")), 0.5);	
	return GridMaterialInstance;
}

UStaticMeshComponent* ATBSGridUI::CreateMeshComponent()
{
	UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(this);
	MeshComponent->SetStaticMesh(GridMesh);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionProfileName(FName(TEXT("GridCollisionProfile")));
	MeshComponent->AttachTo(RootComponent);	
	FinishAndRegisterComponent(MeshComponent);
	return MeshComponent;
}

void ATBSGridUI::SpawnCursor()
{
	GridCursor = GetWorld()->SpawnActor<ATBSGridCursor>(GridCursorClass);
}

void ATBSGridUI::HandleGridHitResult(FHitResult HitResult)
{
	CoordinatesChanged = false;
	CursorOnGrid = false;

	if (HitResult.Actor == nullptr)
	{
		if (PreviousCoordinates != NullCoordinates)
		{
			PreviousCoordinates = CurrentCoordinates;
			CoordinatesChanged = true;
		}

		CurrentCoordinates = NullCoordinates;
		HideCursor();
		return;
	}

	CursorOnGrid = true;
	ShowCursor();

	FVector ActorLocation = HitResult.Actor->GetActorLocation();
	FVector LocalLocation = HitResult.Location - ActorLocation; // Offset by actor location in case actor not in origin

	// Actor location is relative to it's center point; offset by half width/height to get positive local coordinates
	FIntVector NewCoordinates;

	if (CursorDimensions.X % 2 == 0)
	{
		NewCoordinates = FIntVector(
			(int32)((LocalLocation.X + GridMeshWidth / 2) / TileSize) * 10 + SelectionOffset.X,
			(int32)((LocalLocation.Y + GridMeshHeight / 2) / TileSize) * 10 + SelectionOffset.Y,
			(int32)((LocalLocation.Z + 10) / FloorHeight)
		);
	}
	else
	{
		NewCoordinates = FIntVector(
			(int32)((LocalLocation.X + GridMeshWidth / 2) / TileSize) * 10,
			(int32)((LocalLocation.Y + GridMeshHeight / 2) / TileSize) * 10,
			(int32)((LocalLocation.Z + 10) / FloorHeight)
		);
	}

	if (NewCoordinates != PreviousCoordinates)
	{
		CoordinatesChanged = true;
		PreviousCoordinates = CurrentCoordinates;
		CurrentCoordinates = NewCoordinates;		
	}

	if (!CursorForced)
	{
		SetCursorCoordinates(NewCoordinates);
	}	
}

void ATBSGridUI::SetCursorCoordinates(FIntVector Coordinates)
{
	if (GridCursor != nullptr)
	{
		FVector ActorLocation = GetActorLocation();
		FVector TileCenter = FVector(
			((float)Coordinates.X / 10 * TileSize + TileSize / 2) + ActorLocation.X - GridMeshWidth / 2,
			((float)Coordinates.Y / 10 * TileSize + TileSize / 2) + ActorLocation.Y - GridMeshHeight / 2,
			(float)Coordinates.Z * FloorHeight
		);

		GridCursor->SetActorLocation(TileCenter);
	}
}

void ATBSGridUI::LevelUp()
{
	CurrentLevel = FMath::Clamp<int32>(CurrentLevel + 1, 0, Grid->GridDimensions.Z - 1);
	UpdateLevelVisibilities();
}

void ATBSGridUI::LevelDown()
{
	CurrentLevel = FMath::Clamp<int32>(CurrentLevel - 1, 0, Grid->GridDimensions.Z - 1);
	UpdateLevelVisibilities();
}

void ATBSGridUI::UpdateLevelVisibilities()
{
	for (int32 i = 0; i < Grid->GridDimensions.Z; i++)
	{
		if (i == CurrentLevel)
		{
			GridMaterials[i]->SetScalarParameterValue(FName(TEXT("OpacityMultiplier")), 0.5);
			GridMeshes[i]->SetCollisionProfileName(FName(TEXT("GridCollisionProfile")));
		}
		else
		{
			GridMaterials[i]->SetScalarParameterValue(FName(TEXT("OpacityMultiplier")), 0.0);
			GridMeshes[i]->SetCollisionProfileName(FName(TEXT("NoCollision")));
		}
	}
}

void ATBSGridUI::ShowCursor()
{
	GridCursor->SetActorHiddenInGame(false);
}

void ATBSGridUI::HideCursor()
{
	GridCursor->SetActorHiddenInGame(true);
}

FCoordinateLocations ATBSGridUI::GetCoordinateLocations(FIntVector Coordinates)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Asking for coordinate locations (%i, %i, %i)"), Coordinates.X, Coordinates.Y, Coordinates.Z));
	FVector Location = GetActorLocation() - FVector(GridMeshWidth/2, GridMeshHeight/2, 0.0);

	Location = Location + FVector(
		(float)Coordinates.X/10 * TileSize + TileSize/2,
		(float)Coordinates.Y/10 * TileSize + TileSize/2,
		(float)Coordinates.Z/10 * FloorHeight
	);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Asking for coordinate locations (%f, %f, %f)"), Location.X, Location.Y, Location.Z));

	FVector Up = FVector(0.0, TileSize / -2, 0.0);
	FVector Right = FVector(TileSize / 2, 0.0, 0.0);

	FCoordinateLocations Result;
	Result.Center = Location;
	Result.NW = Location + Up - Right;
	Result.N = Location + Up;
	Result.NE = Location + Up + Right;
	Result.E = Location + Right;
	Result.SE = Location - Up - Right;
	Result.S = Location - Up;
	Result.SW = Location - Up - Right;
	Result.W = Location - Right;
	return Result;
}

void ATBSGridUI::SelectLocation(FIntVector Dimensions, FIntVector Coordinates)
{
	if (!SelectionCursor)
	{
		SelectionCursor = GetWorld()->SpawnActor<ATBSGridCursor>(GridCursorClass);
	}

	SelectionCursor->SetActorHiddenInGame(false);
	FVector ActorLocation = GetActorLocation();
	FVector TileCenter = FVector(
		((float)Coordinates.X / 10 * TileSize + TileSize / 2) + ActorLocation.X - GridMeshWidth / 2,
		((float)Coordinates.Y / 10 * TileSize + TileSize / 2) + ActorLocation.Y - GridMeshHeight / 2,
		(float)Coordinates.Z * FloorHeight
	);

	SelectionCursor->SetActorLocation(TileCenter);
	SelectionCursor->SetActorScale3D(FVector(
		(float)Dimensions.X * TileSize / 100,
		(float)Dimensions.Y * TileSize / 100,
		(float)Dimensions.Z * TileSize / 100)
	);
}

void ATBSGridUI::ClearSelection()
{
	SelectionCursor->SetActorHiddenInGame(true);
}

void ATBSGridUI::ResetCursorDimensions()
{
	CursorDimensions = DefaultCursorDimensions;
}

void ATBSGridUI::SetCursorDimensions(FIntVector InCursorDimensions)
{
	CursorDimensions = InCursorDimensions;
	GridCursor->SetActorScale3D(FVector(
		(float)CursorDimensions.X * TileSize / 100,
		(float)CursorDimensions.Y * TileSize / 100,
		(float)CursorDimensions.Z * TileSize / 100)
	);
}

void ATBSGridUI::ForceCursor(FIntVector Dimensions, FIntVector Coordinates)
{
	CursorForced = true;

	if (GridCursor != nullptr)
	{
		FVector ActorLocation = GetActorLocation();
		FVector TileCenter = FVector(
			((float)Coordinates.X / 10 * TileSize + TileSize / 2) + ActorLocation.X - GridMeshWidth / 2,
			((float)Coordinates.Y / 10 * TileSize + TileSize / 2) + ActorLocation.Y - GridMeshHeight / 2,
			(float)Coordinates.Z * FloorHeight
		);

		GridCursor->SetActorLocation(TileCenter);
		GridCursor->SetActorScale3D(FVector(
			(float)Dimensions.X * TileSize / 100,
			(float)Dimensions.Y * TileSize / 100,
			(float)Dimensions.Z * TileSize / 100)
		);
	}
}

void ATBSGridUI::ReleaseCursor()
{
	CursorForced = false;
	SetCursorDimensions(CursorDimensions);
}
