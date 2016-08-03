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

	InitialisePlayerController();
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

void ATBSGridUI::InitialisePlayerController()
{	
	PlayerController = Cast<ATBSPlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
}

void ATBSGridUI::InitialiseParametersFromGrid()
{
	GridWidth = Grid->GridWidth;
	GridHeight = Grid->GridHeight;
	NumOfLevels = Grid->NumOfLevels;
	GridMeshWidth = (float)GridWidth * TileSize;
	GridMeshHeight = (float)GridHeight * TileSize;
}

void ATBSGridUI::CreateGridMaterialInstances()
{
	for (int32 i = 0; i < NumOfLevels; i++)
	{
		UMaterialInstanceDynamic* GridMaterialInstance = CreateMaterialInstance();
		GridMaterials.Push(GridMaterialInstance);
	}
}

void ATBSGridUI::CreateGridMeshComponents()
{
	for (int32 i = 0; i < NumOfLevels; i++)
	{
		UStaticMeshComponent* GridMeshComponent = CreateMeshComponent();
		GridMeshComponent->AddRelativeLocation(FVector(0.0, 0.0, (float)i * FloorHeight));
		GridMeshComponent->SetRelativeScale3D(FVector(GridMeshWidth, GridMeshHeight, 0.01));
		GridMeshComponent->SetMaterial(0, GridMaterials[i]);
		GridMeshes.Push(GridMeshComponent);
	}
}

UMaterialInstanceDynamic* ATBSGridUI::CreateMaterialInstance()
{
	UMaterialInstanceDynamic* GridMaterialInstance = UMaterialInstanceDynamic::Create(GridMaterial, this);
	GridMaterialInstance->SetScalarParameterValue(FName(TEXT("UTiling")), GridWidth + 0.02);
	GridMaterialInstance->SetScalarParameterValue(FName(TEXT("VTiling")), GridHeight + 0.02);
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

// Called every frame
void ATBSGridUI::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FHitResult HitResult = PlayerController->GetGridHitResult();

	if (HitResult.Actor == nullptr)
	{
		// notify cursor off grid
		if (HoverCoordinates != NullVector)
		{
			OnGameTileHoverEnd.Broadcast(HoverCoordinates);
		}

		HoverCoordinates = NullVector;

		HideCursor();
		return;
	}

	ShowCursor();

	FVector ActorLocation = HitResult.Actor->GetActorLocation();
	FVector LocalLocation = HitResult.Location - ActorLocation; // Offset by actor location in case actor not in origin

	// Actor location is relative to it's center point; offset by half width/height to get positive local coordinates
	FIntVector GameCoordinates = FIntVector(
		(int32)((LocalLocation.X + GridMeshWidth / 2) / TileSize),
		(int32)((LocalLocation.Y + GridMeshHeight / 2) / TileSize),
		(int32)((LocalLocation.Z + 10) / FloorHeight)
	);

	FVector TileCenter = FVector(
		((float)GameCoordinates.X * TileSize + TileSize / 2) + ActorLocation.X - GridMeshWidth / 2,
		((float)GameCoordinates.Y * TileSize + TileSize / 2) + ActorLocation.Y - GridMeshHeight / 2,
		(float)GameCoordinates.Z * FloorHeight
	);

	if (GameCoordinates != LastGameCoordinates)
	{
		// notify cursor location change
		HoverCoordinates = GameCoordinates;
		OnGameTileHoverEnd.Broadcast(LastGameCoordinates);
		OnGameTileHoverBegin.Broadcast(GameCoordinates);

		LastGameCoordinates = GameCoordinates;
	}

	UpdateCursorLocation(TileCenter);
}

void ATBSGridUI::UpdateCursorLocation(const FVector Location)
{
	if (GridCursor != nullptr)
	{
		GridCursor->SetActorLocation(Location);
	}
}

void ATBSGridUI::LevelUp()
{
	CurrentLevel = FMath::Clamp<int32>(CurrentLevel + 1, 0, NumOfLevels - 1);
	UpdateLevelVisibilities();
}

void ATBSGridUI::LevelDown()
{
	CurrentLevel = FMath::Clamp<int32>(CurrentLevel - 1, 0, NumOfLevels - 1);
	UpdateLevelVisibilities();
}

int32 ATBSGridUI::GetCurrentLevel()
{
	return CurrentLevel;
}

void ATBSGridUI::UpdateLevelVisibilities()
{
	for (int32 i = 0; i < NumOfLevels; i++)
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

void ATBSGridUI::HandleMouseDown()
{
	// notify click
	if (HoverCoordinates == NullVector)
	{
		return;
	}

	if (SelectCoordinates != NullVector)
	{
		OnGameTileSelectEnd.Broadcast(SelectCoordinates);
	}

	SelectCoordinates = HoverCoordinates;
	OnGameTileSelectBegin.Broadcast(SelectCoordinates);
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
	FVector Location = GetActorLocation() - FVector(GridMeshWidth/2, GridMeshHeight/2, Coordinates.Z * FloorHeight);
	Location = Location + FVector((float)Coordinates.X * TileSize + TileSize/2, (float)Coordinates.Y * TileSize + TileSize/2, 0.0);

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
