// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSDefaultPawn.h"

#include "Kismet/KismetMathLibrary.h"

ATBSDefaultPawn::ATBSDefaultPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	// Clear unnecessary components
	GetCollisionComponent()->DestroyComponent();
	GetMeshComponent()->DestroyComponent();
	ClearInstanceComponents(true);

	// Add scene component as root to allow camera rotation in actor local coordinates
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->SetFieldOfView(35.0);
}

void ATBSDefaultPawn::BeginPlay()
{
	// This seems to work more reliably with the Editor than calling
	// SetupAttachment in the constructor
	CameraComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	SetActorLocation(FVector(0.0, 0.0, 0.0));
	SetActorRotation(FQuat(0.0, 0.0, 0.0, 0.0));
	UpdateCameraPositionAndRotation();
}

void ATBSDefaultPawn::TogglePerspectiveCamera()
{
	if (CameraComponent->ProjectionMode == ECameraProjectionMode::Orthographic)
	{
		CameraComponent->SetProjectionMode(ECameraProjectionMode::Perspective);
	}
	else
	{
		CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
	}
}

void ATBSDefaultPawn::MoveCameraForward(float AxisValue)
{
	if (AxisValue == 0.0 && MoveForwardAxisOffset == 0.0)
	{
		return;
	}

	FVector CameraLocation = CameraComponent->GetRelativeTransform().GetLocation();
	FRotator CameraYawRotation = FRotator(0, CameraComponent->GetComponentRotation().Yaw, 0);
	FVector DeltaVector = UKismetMathLibrary::GetForwardVector(CameraYawRotation);

	float AxisValueTotal = FMath::Clamp<float>(AxisValue + MoveForwardAxisOffset, -1.0, 1.0);

	SetActorLocation(GetActorLocation() + (AxisValueTotal * CameraMovementSpeed * DeltaVector));
}

void ATBSDefaultPawn::MoveCameraRight(float AxisValue)
{
	if (AxisValue == 0.0 && MoveRightAxisOffset == 0.0)
	{
		return;
	}

	FVector CameraLocation = CameraComponent->GetRelativeTransform().GetLocation();
	FRotator CameraYawRotation = FRotator(0, CameraComponent->GetComponentRotation().Yaw, 0);
	FVector DeltaVector = UKismetMathLibrary::GetRightVector(CameraYawRotation);

	float AxisValueTotal = FMath::Clamp<float>(AxisValue + MoveRightAxisOffset, -1.0, 1.0);

	SetActorLocation(GetActorLocation() + (AxisValueTotal * CameraMovementSpeed * DeltaVector));
}

void ATBSDefaultPawn::ViewLevelUp()
{

}

void ATBSDefaultPawn::ViewLevelDown()
{

}

void ATBSDefaultPawn::RotateCameraRight()
{
	CameraViewAngleDeg = CameraViewAngleDeg - 90.0;
	if (CameraViewAngleDeg < 0)
	{
		CameraViewAngleDeg = 315;
	}
	UpdateCameraPositionAndRotation();
}

void ATBSDefaultPawn::RotateCameraLeft()
{
	CameraViewAngleDeg = CameraViewAngleDeg + 90.0;

	if (CameraViewAngleDeg > 315)
	{
		CameraViewAngleDeg = 45;
	}

	UpdateCameraPositionAndRotation();
}

void ATBSDefaultPawn::ZoomCameraIn()
{
	CameraDistance = FMath::Clamp(CameraDistance - CameraZoomSpeed, CameraDistanceMin, CameraDistanceMax);
	UpdateCameraPositionAndRotation();
}

void ATBSDefaultPawn::ZoomCameraOut()
{
	CameraDistance = FMath::Clamp(CameraDistance + CameraZoomSpeed, CameraDistanceMin, CameraDistanceMax);
	UpdateCameraPositionAndRotation();
}

void ATBSDefaultPawn::UpdateCameraPositionAndRotation()
{
	CameraComponent->SetRelativeLocation(CalculateCameraPosition());
	CameraComponent->SetRelativeRotation(CalculateCameraRotation());
}

FVector ATBSDefaultPawn::CalculateCameraPosition()
{
	FTransform CameraTransform = CameraComponent->GetRelativeTransform();
	FVector CameraLocation = CameraTransform.GetLocation();

	double CameraViewAngleRad = PI / 180.0  * CameraViewAngleDeg;
	double CameraHeightAngleRad = PI / 180.0 * CameraHeightAngleDeg;

	float X = CameraDistance * cos(CameraViewAngleRad) * sin(CameraHeightAngleRad);
	float Y = CameraDistance * sin(CameraViewAngleRad) * sin(CameraHeightAngleRad);
	float Z = CameraDistance * cos(CameraHeightAngleRad);

	return FVector(X, Y, Z);
}

FQuat ATBSDefaultPawn::CalculateCameraRotation()
{
	FVector Origin = FVector(0.0, 0.0, 0.0);
	FRotator CameraRotator = FRotationMatrix::MakeFromX(Origin - CameraComponent->RelativeLocation).Rotator();
	return CameraRotator.Quaternion();
}

void ATBSDefaultPawn::SetMoveForwardAxisOffset(float Offset)
{
	MoveForwardAxisOffset = Offset;
}

void ATBSDefaultPawn::SetMoveRightAxisOffset(float Offset)
{
	MoveRightAxisOffset = Offset;
}
