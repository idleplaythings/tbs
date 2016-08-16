// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSPlayerController.h"

ATBSPlayerController::ATBSPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	EnableMouse();
}

void ATBSPlayerController::EnableMouse()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
	bEnableTouchOverEvents = true;
}

void ATBSPlayerController::Initialise()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Controller initialise")));

	DefaultPawn = Cast<ATBSDefaultPawn>(GetPawn());
	GridUI = GetGridUI();
}

void ATBSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Setting up input component in player controller")));

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

ATBSGridUI* ATBSPlayerController::GetGridUI()
{
	for (TActorIterator<ATBSGridUI> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		return *ActorItr;
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("No grid ui actor found")));

	return nullptr;
}
