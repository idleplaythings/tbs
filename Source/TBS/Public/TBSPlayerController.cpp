// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSPlayerState.h"
#include "TBSPlayerController.h"

ATBSPlayerController::ATBSPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	EnableMouse();

	//bAutoManageActiveCameraTarget = false;
}

void ATBSPlayerController::EnableMouse()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
	bEnableTouchOverEvents = true;
}

void ATBSPlayerController::BeginPlay()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSPlayerController::BeginPlay")));

	if (IsLocalPlayerController())
	{
		PlayerContext = FLocalPlayerContext(this);
		GetWorldTimerManager().SetTimer(InitTimer, this, &ATBSPlayerController::InitLocalClasses, 0.2f, true);
	}		
}

void ATBSPlayerController::SetupInputComponent()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSPlayerController::SetupInputComponent")));

	Super::SetupInputComponent();

	InputComponent->BindAction("ActionLevelUp", IE_Pressed, this, &ATBSPlayerController::MoveLevelUp);
	InputComponent->BindAction("ActionLevelDown", IE_Pressed, this, &ATBSPlayerController::MoveLevelDown);
	InputComponent->BindAction("ActionRotateCameraRight", IE_Pressed, this, &ATBSPlayerController::TurnCameraRight);
	InputComponent->BindAction("ActionRotateCameraLeft", IE_Pressed, this, &ATBSPlayerController::TurnCameraLeft);
	InputComponent->BindAction("ActionZoomIn", IE_Pressed, this, &ATBSPlayerController::ZoomCameraIn);
	InputComponent->BindAction("ActionZoomOut", IE_Pressed, this, &ATBSPlayerController::ZoomCameraOut);
	InputComponent->BindAction("TogglePerspectiveCamera", IE_Pressed, this, &ATBSPlayerController::TogglePerspectiveCamera);
	InputComponent->BindAction("ActionMouseLeft", IE_Pressed, this, &ATBSPlayerController::OnMouseLeft);
	InputComponent->BindAction("ActionMouseRight", IE_Pressed, this, &ATBSPlayerController::OnMouseRight);
	InputComponent->BindAxis("AxisMoveCameraForward", this, &ATBSPlayerController::MoveCameraForward);
	InputComponent->BindAxis("AxisMoveCameraRight", this, &ATBSPlayerController::MoveCameraRight);	
}

void ATBSPlayerController::MoveLevelUp()
{
	if (DefaultPawn)
	{
		DefaultPawn->MoveLevelUp();
	}
	
	if (GridUI)
	{
		GridUI->LevelUp();
	}	
}

void ATBSPlayerController::MoveLevelDown()
{
	if (DefaultPawn)
	{
		DefaultPawn->MoveLevelDown();
	}

	if (GridUI)
	{
		GridUI->LevelDown();
	}
}

void ATBSPlayerController::TurnCameraRight()
{
	if (DefaultPawn)
	{
		DefaultPawn->TurnCameraRight();
	}
}

void ATBSPlayerController::TurnCameraLeft()
{
	if (DefaultPawn)
	{
		DefaultPawn->TurnCameraLeft();
	}
}

void ATBSPlayerController::ZoomCameraIn()
{
	if (DefaultPawn)
	{
		DefaultPawn->ZoomCameraIn();
	}
}

void ATBSPlayerController::ZoomCameraOut()
{
	if (DefaultPawn)
	{
		DefaultPawn->ZoomCameraOut();
	}
}

void ATBSPlayerController::TogglePerspectiveCamera()
{
	if (DefaultPawn)
	{
		DefaultPawn->TogglePerspectiveCamera();
	}
}

void ATBSPlayerController::MoveCameraForward(float AxisValue)
{
	DefaultPawn = Cast<ATBSDefaultPawn>(GetPawn());

	if (DefaultPawn)
	{
		DefaultPawn->MoveCameraForward(AxisValue);
	}
}

void ATBSPlayerController::MoveCameraRight(float AxisValue)
{
	if (DefaultPawn)
	{
		DefaultPawn->MoveCameraRight(AxisValue);
	}
}

void ATBSPlayerController::OnMouseLeft()
{
	if (Role == ROLE_Authority)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSPlayerController::OnMouseLeft ROLE_Authority")));
	}
	else if (Role == ROLE_AutonomousProxy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSPlayerController::OnMouseLeftd ROLE_AutonomousProxy")));
	}
	else if (Role == ROLE_SimulatedProxy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSPlayerController::OnMouseLeftd ROLE_SimulatedProxy")));
	}
	else if (Role == ROLE_None)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSPlayerController::OnMouseLeft ROLE_None")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSPlayerController::OnMouseLeft Other")));
	}

	if (GridUI)
	{
		GridUI->HandleMouseLeft();
	}
}

void ATBSPlayerController::OnMouseRight()
{
	if (GridUI)
	{
		GridUI->HandleMouseRight();
	}
}

FHitResult ATBSPlayerController::GetGridHitResult()
{
	FHitResult Result;
	bool bHitSomething = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, true, Result);
	return Result;
}
//
//void ATBSPlayerController::Server_Possess_Implementation(ATBSUnit* Unit)
//{
//	if (HasAuthority())
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Possessing on server")));
//		Possess(Unit);
//	}
//}
//
//bool ATBSPlayerController::Server_Possess_Validate(ATBSUnit* Unit)
//{
//	return true;
//}
//
//void ATBSPlayerController::Server_UnPossess_Implementation()
//{
//	if (HasAuthority())
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Unpossessing on server")));
//		UnPossess();
//	}
//}
//
//bool ATBSPlayerController::Server_UnPossess_Validate()
//{
//	return true;
//}

void ATBSPlayerController::InitLocalClasses()
{
	if (DefaultPawn && Grid && GridUI && PlayerState2)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("All local classes Initialised")));

		GridUI->OnGameTileMouseLeft.AddDynamic(PlayerState2, &ATBSPlayerState::MouseLeft);
		GridUI->OnGameTileMouseRight.AddDynamic(PlayerState2, &ATBSPlayerState::MouseRight);
		GridUI->OnGameTileHoverBegin.AddDynamic(PlayerState2, &ATBSPlayerState::HoverBegin);
		GridUI->OnGameTileHoverEnd.AddDynamic(PlayerState2, &ATBSPlayerState::HoverEnd);

		GetWorldTimerManager().ClearTimer(InitTimer);
		return;
	}

	InitDefaultPawn();
	InitGrid();
	InitGridUI();
	InitPlayerState2();
}

bool ATBSPlayerController::InitDefaultPawn()
{
	if (DefaultPawn)
	{
		return true;
	}

	DefaultPawn = Cast<ATBSDefaultPawn>(GetPawn());

	return true;
}

bool ATBSPlayerController::InitGrid()
{
	if (Grid)
	{
		return true;
	}

	for (TActorIterator<ATBSGrid> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		Grid = *ActorItr;
		return true;
	}

	return false;
}

bool ATBSPlayerController::InitGridUI()
{
	if (!InitGrid())
	{
		return false;
	}

	if (GridUI)
	{
		return true;
	}

	GridUI = GetWorld()->SpawnActor<ATBSGridUI>(ATBSGridUI::StaticClass());
	GridUI->RenderGrid(Grid);
	return true;
}

bool ATBSPlayerController::InitPlayerState2()
{
	if (!InitGrid())
	{
		return false;
	}

	if (!InitGridUI())
	{
		return false;
	}

	PlayerState2 = Cast<ATBSPlayerState>(PlayerState);
	PlayerState2->Initialise(Grid, GridUI);

	return true;
}

