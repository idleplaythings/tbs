// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DefaultPawn.h"
#include "TBSGrid.h"
#include "TBSGridCursor.h"
#include "TBSDefaultPawn.generated.h"

/**
 *
 */
UCLASS()
class TBS_API ATBSDefaultPawn : public ADefaultPawn
{
	GENERATED_BODY()

	ATBSDefaultPawn();

public:
	void BeginPlay() override;
	void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	USceneComponent* SceneComponent;

	UPROPERTY(Category = Camera, VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UClass* GridCursorClass;
	ATBSGridCursor* GridCursor;





	ATBSGrid* Grid;

	void MoveCameraForward(float AxisValue);
	void MoveCameraRight(float AxisValue);
	void MoveLevelUp();
	void MoveLevelDown();
	void TurnCameraRight();
	void TurnCameraLeft();
	void ZoomCameraIn();
	void ZoomCameraOut();
	void TogglePerspectiveCamera();
	void MouseAction();

	float MoveForwardAxisOffset = 0.0;
	float MoveRightAxisOffset = 0.0;

private:
	void UpdateCameraPositionAndRotation();
	FVector CalculateCameraPosition();
	FQuat CalculateCameraRotation();

	float CameraViewAngleDeg = 45.0;
	float CameraHeightAngleDeg = 45.0;
	float CameraDistance = 4000;
	float CameraMovementSpeed = 12.0;
	float CameraDistanceMin = 200.0;
	float CameraDistanceMax = 10000.0;
	float CameraZoomSpeed = 200.0;
	float CameraOrtographicWidth = 2000.0;
	int CameraLevel = 0;
};
