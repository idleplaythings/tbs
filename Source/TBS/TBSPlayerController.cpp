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

void ATBSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	DefaultPawn = Cast<ATBSDefaultPawn>(GetPawn());
	GridUI = GetGridUI();
}

void ATBSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("ActionLevelUp", IE_Pressed, this, &ATBSPlayerController::MoveLevelUp);
	InputComponent->BindAction("ActionLevelDown", IE_Pressed, this, &ATBSPlayerController::MoveLevelDown);
	InputComponent->BindAction("ActionRotateCameraRight", IE_Pressed, this, &ATBSPlayerController::TurnCameraRight);
	InputComponent->BindAction("ActionRotateCameraLeft", IE_Pressed, this, &ATBSPlayerController::TurnCameraLeft);
	InputComponent->BindAction("ActionZoomIn", IE_Pressed, this, &ATBSPlayerController::ZoomCameraIn);
	InputComponent->BindAction("ActionZoomOut", IE_Pressed, this, &ATBSPlayerController::ZoomCameraOut);
	InputComponent->BindAction("TogglePerspectiveCamera", IE_Pressed, this, &ATBSPlayerController::TogglePerspectiveCamera);
	InputComponent->BindAction("ActionMouseDown", IE_Pressed, this, &ATBSPlayerController::OnMouseDown);
	InputComponent->BindAxis("AxisMoveCameraForward", this, &ATBSPlayerController::MoveCameraForward);
	InputComponent->BindAxis("AxisMoveCameraRight", this, &ATBSPlayerController::MoveCameraRight);	
}

void ATBSPlayerController::MoveLevelUp()
{
	DefaultPawn->MoveLevelUp();
	GridUI->LevelUp();
}

void ATBSPlayerController::MoveLevelDown()
{
	DefaultPawn->MoveLevelDown();
	GridUI->LevelDown();
}

void ATBSPlayerController::TurnCameraRight()
{
	DefaultPawn->TurnCameraRight();
}

void ATBSPlayerController::TurnCameraLeft()
{
	DefaultPawn->TurnCameraLeft();
}

void ATBSPlayerController::ZoomCameraIn()
{
	DefaultPawn->ZoomCameraIn();
}

void ATBSPlayerController::ZoomCameraOut()
{
	DefaultPawn->ZoomCameraOut();
}

void ATBSPlayerController::TogglePerspectiveCamera()
{
	DefaultPawn->TogglePerspectiveCamera();
}

void ATBSPlayerController::MoveCameraForward(float AxisValue)
{
	DefaultPawn->MoveCameraForward(AxisValue);
}

void ATBSPlayerController::MoveCameraRight(float AxisValue)
{
	DefaultPawn->MoveCameraRight(AxisValue);
}

void ATBSPlayerController::OnMouseDown()
{
	GridUI->HandleMouseDown();
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

	return nullptr;
}
