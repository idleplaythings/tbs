// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSDefaultPawn.h"
#include "TBSPlayerController.h"

#include "Kismet/KismetMathLibrary.h"

ATBSDefaultPawn::ATBSDefaultPawn()
{
	PrimaryActorTick.bCanEverTick = true;

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
	CameraComponent->AttachTo(RootComponent);

	SetActorLocation(FVector(0.0, 0.0, 0.0));
	SetActorRotation(FQuat(0.0, 0.0, 0.0, 0.0));
	UpdateCameraPositionAndRotation();

	//GridCursor = (ATBSGridCursor*)GetWorld()->SpawnActor(ATBSGridCursor::StaticClass(), FName(TEXT("GridCursor")));
	UClass* C = GetClass();

	GridCursor = GetWorld()->SpawnActor<ATBSGridCursor>(C);
}

void ATBSDefaultPawn::SetupPlayerInputComponent(UInputComponent * InputComponent)
{
	InputComponent->BindAction("ActionLevelUp", IE_Pressed, this, &ATBSDefaultPawn::MoveLevelUp);
	InputComponent->BindAction("ActionLevelDown", IE_Pressed, this, &ATBSDefaultPawn::MoveLevelDown);
	InputComponent->BindAction("ActionRotateCameraRight", IE_Pressed, this, &ATBSDefaultPawn::TurnCameraRight);
	InputComponent->BindAction("ActionRotateCameraLeft", IE_Pressed, this, &ATBSDefaultPawn::TurnCameraLeft);
	InputComponent->BindAction("ActionZoomIn", IE_Pressed, this, &ATBSDefaultPawn::ZoomCameraIn);
	InputComponent->BindAction("ActionZoomOut", IE_Pressed, this, &ATBSDefaultPawn::ZoomCameraOut);
	InputComponent->BindAction("TogglePerspectiveCamera", IE_Pressed, this, &ATBSDefaultPawn::TogglePerspectiveCamera);

	InputComponent->BindAction("ActionMouse", IE_Pressed, this, &ATBSDefaultPawn::MouseAction);

	InputComponent->BindAxis("AxisMoveCameraForward", this, &ATBSDefaultPawn::MoveCameraForward);
	InputComponent->BindAxis("AxisMoveCameraRight", this, &ATBSDefaultPawn::MoveCameraRight);
}

void ATBSDefaultPawn::MouseAction()
{
	//AController* Controller = GetController();	

	//FHitResult Result;

	////bool bHitSomething = Cast<ATBSPlayerController>(Controller)->GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, true, Result);
	//bool bHitSomething = Cast<ATBSPlayerController>(Controller)->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, true, Result);

	//if (bHitSomething)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Hit something! (%f, %f, %f)"), Result.Location.X, Result.Location.Y, Result.Location.Z));
	//}	
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

void ATBSDefaultPawn::MoveLevelUp()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString(TEXT("Level Up")));
	CameraLevel = CameraLevel + 1;
}

void ATBSDefaultPawn::MoveLevelDown()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString(TEXT("Level Down")));
	CameraLevel = CameraLevel - 1;
}

void ATBSDefaultPawn::TurnCameraRight()
{
	CameraViewAngleDeg = CameraViewAngleDeg - 90.0;
	if (CameraViewAngleDeg < 0)
	{
		CameraViewAngleDeg = 315;
	}
	UpdateCameraPositionAndRotation();
}

void ATBSDefaultPawn::TurnCameraLeft()
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
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString(TEXT("Zoom In")));

	CameraDistance = FMath::Clamp(CameraDistance - CameraZoomSpeed, CameraDistanceMin, CameraDistanceMax);
	UpdateCameraPositionAndRotation();
}

void ATBSDefaultPawn::ZoomCameraOut()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString(TEXT("Zoom Out")));

	CameraDistance = FMath::Clamp(CameraDistance + CameraZoomSpeed, CameraDistanceMin, CameraDistanceMax);
	UpdateCameraPositionAndRotation();
}

void ATBSDefaultPawn::UpdateCameraPositionAndRotation()
{
	CameraComponent->SetRelativeLocation(CalculateCameraPosition());
	CameraComponent->SetRelativeRotation(CalculateCameraRotation());

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString(TEXT("Camera View Angle: ")).Append(FString::SanitizeFloat(CameraViewAngleDeg)));

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString(TEXT("Camera Height Angle: ")).Append(FString::SanitizeFloat(CameraHeightAngleDeg)));

}

FVector ATBSDefaultPawn::CalculateCameraPosition()
{
	FTransform CameraTransform = CameraComponent->GetRelativeTransform();

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		FString(FString::SanitizeFloat((double)GetActorLocation().X)
			.Append(FString(TEXT(",")))
			.Append(FString::SanitizeFloat((double)GetActorLocation().Y))
		)
	);

	FVector CameraLocation = CameraTransform.GetLocation();

	double CameraViewAngleRad = PI / 180.0  * CameraViewAngleDeg;
	double CameraHeightAngleRad = PI / 180.0 * CameraHeightAngleDeg;

	float X = CameraDistance * cos(CameraViewAngleRad) * sin(CameraHeightAngleRad);
	float Y = CameraDistance * sin(CameraViewAngleRad) * sin(CameraHeightAngleRad);
	float Z = CameraDistance * cos(CameraHeightAngleRad);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString(TEXT("Camera: "))
		.Append(FString::SanitizeFloat(X))
		.Append(FString(TEXT(",")))
		.Append(FString::SanitizeFloat(Y))
		.Append(FString(TEXT(",")))
		.Append(FString::SanitizeFloat(Z))
	);

	return FVector(X, Y, Z);
}

FQuat ATBSDefaultPawn::CalculateCameraRotation()
{
	FVector Origin = FVector(0.0, 0.0, 0.0);
	FRotator CameraRotator = FRotationMatrix::MakeFromX(Origin - CameraComponent->RelativeLocation).Rotator();
	return CameraRotator.Quaternion();
}
