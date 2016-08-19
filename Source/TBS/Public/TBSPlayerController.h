// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "TBSGrid.h"
#include "TBSGridUI.h"
#include "TBSPlayerState.h"
#include "TBSDefaultPawn.h"
#include "TBSPlayerController.generated.h"

UCLASS()
class TBS_API ATBSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATBSPlayerController(const FObjectInitializer& ObjectInitializer);
	void EnableMouse();
	void BeginPlay();
	void SetupInputComponent();
	void MoveLevelUp();
	void MoveLevelDown();
	void TurnCameraRight();
	void TurnCameraLeft();
	void ZoomCameraIn();
	void ZoomCameraOut();
	void TogglePerspectiveCamera();
	void MoveCameraForward(float AxisValue);
	void MoveCameraRight(float AxisValue);
	void OnMouseLeft();
	void OnMouseRight();
	FHitResult GetGridHitResult();
	void Server_Possess_Implementation(ATBSUnit* Unit);
	bool Server_Possess_Validate(ATBSUnit* Unit);
	void Server_UnPossess_Implementation();
	bool Server_UnPossess_Validate();
	void InitLocalClasses();
	bool InitDefaultPawn();
	bool InitGrid();
	bool InitGridUI();
	bool InitPlayerState2();

	ATBSGrid* Grid;
	ATBSGridUI* GridUI;
	//ATBSPlayerState* PlayerState;
	ATBSPlayerState* PlayerState2;
	ATBSDefaultPawn* DefaultPawn;
	FLocalPlayerContext PlayerContext;
	FTimerHandle InitTimer;
};

