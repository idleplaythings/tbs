// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSGameMode.h"
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
	InitialiseParametersFromGameMode();
	CreateGridMaterialInstances();
	CreateGridMeshComponents();
	UpdateLevelVisibilities();
	SpawnCursor();
}

void ATBSGridUI::InitialisePlayerController()
{
	PlayerController = Cast<ATBSPlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
}

void ATBSGridUI::InitialiseParametersFromGameMode()
{
	ATBSGameMode* GameMode = Cast<ATBSGameMode>(GetWorld()->GetAuthGameMode());

	GridWidth = GameMode->GridWidth;
	GridHeight = GameMode->GridHeight;
	//TileSize = GameMode->TileSize;
	//FloorHeight = GameMode->FloorHeight;
	NumOfLevels = GameMode->NumOfLevels;
	GridMeshWidth = (float)GridWidth * TileSize;
	GridMeshHeight = (float)GridHeight * TileSize;
}

void ATBSGridUI::CreateGridMaterialInstances()
{
	for (int i = 0; i < NumOfLevels; i++)
	{
		UMaterialInstanceDynamic* GridMaterialInstance = CreateMaterialInstance();
		GridMaterials.Push(GridMaterialInstance);
	}
}

void ATBSGridUI::CreateGridMeshComponents()
{
	for (int i = 0; i < NumOfLevels; i++)
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

	if (PlayerController == nullptr)
	{
		return;
	}

	FHitResult Result;
	bool bHitSomething = PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, true, Result);

	if (!bHitSomething)
	{
		return;
	}

	FVector ActorLocation = Result.Actor->GetActorLocation();
	FVector LocalLocation = Result.Location - ActorLocation; // Offset by actor location in case actor not in origin

	// Actor location is relative to it's center point; offset by half width/height to get positive local coordinates
	FIntVector GameCoordinates = FIntVector(
		(int)((LocalLocation.X + GridMeshWidth / 2) / TileSize),
		(int)((LocalLocation.Y + GridMeshHeight / 2) / TileSize),
		(int)((LocalLocation.Z + 10) / FloorHeight)
	);

	FVector TileCenter = FVector(
		((float)GameCoordinates.X * TileSize + TileSize / 2) + ActorLocation.X - GridMeshWidth / 2,
		((float)GameCoordinates.Y * TileSize + TileSize / 2) + ActorLocation.Y - GridMeshWidth / 2,
		(float)GameCoordinates.Z * FloorHeight
	);

	if (GameCoordinates != LastGameCoordinates)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Game Coordinates (%i, %i, %i)"), GameCoordinates.X, GameCoordinates.Y, GameCoordinates.Z));
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
	CurrentLevel = FMath::Clamp<int>(CurrentLevel + 1, 0, NumOfLevels - 1);
	UpdateLevelVisibilities();
}

void ATBSGridUI::LevelDown()
{
	CurrentLevel = FMath::Clamp<int>(CurrentLevel - 1, 0, NumOfLevels - 1);
	UpdateLevelVisibilities();
}

int ATBSGridUI::GetCurrentLevel()
{
	return CurrentLevel;
}

void ATBSGridUI::UpdateLevelVisibilities()
{
	for (int i = 0; i < NumOfLevels; i++)
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
