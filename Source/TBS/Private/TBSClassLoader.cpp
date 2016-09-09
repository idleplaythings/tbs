// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSPlayerController.h"
#include "TBSClassLoader.h"


// Sets default values
ATBSClassLoader::ATBSClassLoader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATBSClassLoader::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBSClassLoader::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATBSClassLoader::Initialise(ATBSPlayerController* InPlayerController)
{
	PlayerController = InPlayerController;

	GetWorldTimerManager().SetTimer(InitTimer, this, &ATBSClassLoader::TryLoadClasses, 0.2f, true);
}

void ATBSClassLoader::TryLoadClasses()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSPlayerController::LoadLocalClasses")));

	if (DefaultPawn && Grid && GridUI && GridPathFinder && GridPathRenderer && PropFactory && PropManager)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("All local classes Initialised")));

		GetWorldTimerManager().ClearTimer(InitTimer);
		OnClassesLoaded.Broadcast();
		return;
	}

	if (!DefaultPawn)
	{
		DefaultPawn = Cast<ATBSDefaultPawn>(PlayerController->GetPawn());
	}

	if (!Grid)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("No grid")));

		for (TActorIterator<ATBSGrid> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			Grid = *ActorItr;
		}
	}

	if (Grid && !GridUI)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("No grid ui")));

		GridUI = GetWorld()->SpawnActor<ATBSGridUI>(ATBSGridUI::StaticClass());
		GridUI->RenderGrid(Grid);
	}

	if (Grid && GridUI)
	{
		GridPathFinder = GetWorld()->SpawnActor<ATBSGridPathFinder>(ATBSGridPathFinder::StaticClass());
		GridPathFinder->Initialise(Grid);

		GridPathRenderer = GetWorld()->SpawnActor<ATBSGridPathRenderer>(ATBSGridPathRenderer::StaticClass());
		GridPathRenderer->Initialise(Grid, GridUI);

		PropFactory = GetWorld()->SpawnActor<ATBSPropFactory>(ATBSPropFactory::StaticClass());
		PropManager = GetWorld()->SpawnActor<ATBSPropManager>(ATBSPropManager::StaticClass());
		PropManager->Initialise(Grid, GridUI);
	}
}
