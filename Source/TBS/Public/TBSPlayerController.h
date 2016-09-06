// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "TBSClassLoader.h"
#include "TBSUIContextStack.h"
#include "TBSGrid.h"
#include "TBSGridUI.h"
#include "TBSGridPathFinder.h"
#include "TBSGridPathRenderer.h"
#include "TBSPlayerState.h"
#include "TBSDefaultPawn.h"
#include "TBSTypes.h"
#include "TBSPlayerController.generated.h"

UCLASS()
class TBS_API ATBSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATBSPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void PlayerTick(float DeltaSeconds) override;

	void EnableMouse();
	void BeginPlay();

	UPROPERTY(Replicated)
	int32 PlayerNumber = 0;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_HandleCommand(ATBSUnit* Unit, const TArray<FIntVector>& Movements);

	UFUNCTION()
	void OnClassesLoaded();

	ATBSUnit* SelectedUnit;
	TArray<FIntVector> SelectedPath;
	bool PathSelected = false;

	void ViewLevelUp();
	void ViewLevelDown();
	void RotateCameraRight();
	void RotateCameraLeft();
	void ZoomCameraIn();
	void ZoomCameraOut();
	void TogglePerspectiveCamera();
	void MoveCameraForward(float AxisValue);
	void MoveCameraRight(float AxisValue);
	void MouseLeft();
	void MouseRight();
	void Escape();
	void MoveCameraForwardOffset(float AxisValue);
	void MoveCameraRightOffset(float AxisValue);

private:
	TBSUIContextStack* UIContextStack;
	ATBSClassLoader* ClassLoader;
	bool ClassesLoaded = false;
};

