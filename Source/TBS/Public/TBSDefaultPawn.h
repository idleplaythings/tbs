// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DefaultPawn.h"
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
	void MoveCameraForward(float AxisValue);
	void MoveCameraRight(float AxisValue);
	void ViewLevelUp();
	void ViewLevelDown();
	void RotateCameraRight();
	void RotateCameraLeft();
	void ZoomCameraIn();
	void ZoomCameraOut();
	void TogglePerspectiveCamera();
	void SetMoveForwardAxisOffset(float Offset);
	void SetMoveRightAxisOffset(float Offset);
	float GetCameraViewAngle();

private:
	USceneComponent* SceneComponent;
	UCameraComponent* CameraComponent;

	void UpdateCameraPositionAndRotation();
	FVector CalculateCameraPosition();
	FQuat CalculateCameraRotation();

	float CameraViewAngleDeg = 135;
	float CameraHeightAngleDeg = 45.0;
	float CameraDistance = 4000;
	float CameraMovementSpeed = 12.0;
	float CameraDistanceMin = 200.0;
	float CameraDistanceMax = 10000.0;
	float CameraZoomSpeed = 200.0;
	float CameraOrtographicWidth = 2000.0;
	int32 CameraLevel = 0;
	float MoveForwardAxisOffset = 0.0;
	float MoveRightAxisOffset = 0.0;
};
