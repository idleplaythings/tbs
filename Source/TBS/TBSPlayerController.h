// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "TBSDefaultPawn.h"
#include "TBSGridUI.h"
#include "TBSPlayerController.generated.h"

/**
 *
 */
UCLASS()
class TBS_API ATBSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATBSPlayerController(const FObjectInitializer& ObjectInitializer);

	ATBSDefaultPawn* DefaultPawn;
	ATBSGridUI* GridUI;

	void SetupInputComponent() override;
	void BeginPlay() override;

	void OnMouseLeft();	
	void OnMouseRight();
	void MoveLevelUp();
	void MoveLevelDown();
	void MoveCameraForward(float AxisValue);
	void MoveCameraRight(float AxisValue);
	void TurnCameraLeft();
	void TurnCameraRight();
	void ZoomCameraIn();
	void ZoomCameraOut();
	void TogglePerspectiveCamera();
	FHitResult GetGridHitResult();

private:
	void EnableMouse();
	ATBSGridUI* GetGridUI();
};
