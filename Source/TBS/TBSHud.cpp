// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSHUD.h"

const FName ATBSHUD::ScrollHitBoxNW = FName("ScrollHitBoxNW");
const FName ATBSHUD::ScrollHitBoxN = FName("ScrollHitBoxN");
const FName ATBSHUD::ScrollHitBoxNE = FName("ScrollHitBoxNE");
const FName ATBSHUD::ScrollHitBoxE = FName("ScrollHitBoxE");
const FName ATBSHUD::ScrollHitBoxSE = FName("ScrollHitBoxSE");
const FName ATBSHUD::ScrollHitBoxS = FName("ScrollHitBoxS");
const FName ATBSHUD::ScrollHitBoxSW = FName("ScrollHitBoxSW");
const FName ATBSHUD::ScrollHitBoxW = FName("ScrollHitBoxW");

ATBSHUD::ATBSHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void ATBSHUD::BeginPlay()
{
	Pawn = Cast<ATBSDefaultPawn>(GetOwningPawn());
}

void ATBSHUD::DrawHUD()
{
	Super::DrawHUD();
	CalculateScrollHitBoxSizesAndLocations();
	AddScrollHitBoxes();
	DrawScrollHitBoxOverlays();
}

void ATBSHUD::CalculateScrollHitBoxSizesAndLocations()
{
	const float NewCanvasX = (float) Canvas->SizeX;
	const float NewCanvasY = (float) Canvas->SizeY;

	if (NewCanvasX == CanvasX && NewCanvasY == CanvasY) {
		return;
	}

	CanvasX = NewCanvasX;
	CanvasY = NewCanvasY;
	
	HitBoxes.Empty();

	HitBox TopLeft, TopCenter, TopRight, MiddleRight, BottomRight, BottomCenter, BottomLeft, MiddleLeft;

	TopLeft.Position = FVector2D(0, 0);
	TopLeft.Size = FVector2D(HitBoxWidth, HitBoxWidth);
	TopLeft.Name = ATBSHUD::ScrollHitBoxNW;
	HitBoxes.Add(TopLeft);

	TopCenter.Position = FVector2D(HitBoxWidth, 0);
	TopCenter.Size = FVector2D(CanvasX - 2 * HitBoxWidth, HitBoxWidth);
	TopCenter.Name = ATBSHUD::ScrollHitBoxN;
	HitBoxes.Add(TopCenter);

	TopRight.Position = FVector2D(CanvasX - HitBoxWidth, 0);
	TopRight.Size = TopLeft.Size;
	TopRight.Name = ATBSHUD::ScrollHitBoxNE;
	HitBoxes.Add(TopRight);

	MiddleRight.Position = FVector2D(CanvasX - HitBoxWidth, HitBoxWidth);
	MiddleRight.Size = FVector2D(HitBoxWidth, CanvasY - 2 * HitBoxWidth);
	MiddleRight.Name = ATBSHUD::ScrollHitBoxE;
	HitBoxes.Add(MiddleRight);

	BottomRight.Position = FVector2D(CanvasX - HitBoxWidth, CanvasY - HitBoxWidth);
	BottomRight.Size = TopLeft.Size;
	BottomRight.Name = ATBSHUD::ScrollHitBoxSE;
	HitBoxes.Add(BottomRight);

	BottomCenter.Position = FVector2D(HitBoxWidth, CanvasY - HitBoxWidth);
	BottomCenter.Size = TopCenter.Size;
	BottomCenter.Name = ATBSHUD::ScrollHitBoxS;
	HitBoxes.Add(BottomCenter);

	BottomLeft.Position = FVector2D(0, CanvasY - HitBoxWidth);
	BottomLeft.Size = TopLeft.Size;
	BottomLeft.Name = ATBSHUD::ScrollHitBoxSW;
	HitBoxes.Add(BottomLeft);

	MiddleLeft.Position = FVector2D(0, HitBoxWidth);
	MiddleLeft.Size = MiddleRight.Size;
	MiddleLeft.Name = ATBSHUD::ScrollHitBoxW;
	HitBoxes.Add(MiddleLeft);
}

void ATBSHUD::AddScrollHitBoxes()
{
	for (auto& HitBox : HitBoxes)
	{
		AddHitBox(HitBox.Position, HitBox.Size, HitBox.Name, ConsumeScrollInput, ScrollHitBoxPriority);
	}
}

void ATBSHUD::DrawScrollHitBoxOverlays()
{
	if (DrawScrollOverlay)
	{
		for (auto& HitBox : HitBoxes)
		{
			DrawRect(GreenColor, HitBox.Position.X, HitBox.Position.Y, HitBox.Size.X, HitBox.Size.Y);
		}
	}
}

void ATBSHUD::NotifyHitBoxBeginCursorOver(FName BoxName)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString(TEXT("Begin: ")).Append(BoxName.ToString()));

	if (BoxName.IsEqual(ATBSHUD::ScrollHitBoxNW))
	{
		Pawn->MoveForwardAxisOffset = 1.0;
		Pawn->MoveRightAxisOffset = -1.0;
	}
	else if (BoxName.IsEqual(ATBSHUD::ScrollHitBoxN))
	{
		Pawn->MoveForwardAxisOffset = 1.0;
		Pawn->MoveRightAxisOffset = 0.0;
	}
	else if (BoxName.IsEqual(ATBSHUD::ScrollHitBoxNE))
	{
		Pawn->MoveForwardAxisOffset = 1.0;
		Pawn->MoveRightAxisOffset = 1.0;
	}
	else if (BoxName.IsEqual(ATBSHUD::ScrollHitBoxE))
	{
		Pawn->MoveForwardAxisOffset = 0.0;
		Pawn->MoveRightAxisOffset = 1.0;
	}
	else if (BoxName.IsEqual(ATBSHUD::ScrollHitBoxSE))
	{
		Pawn->MoveForwardAxisOffset = -1.0;
		Pawn->MoveRightAxisOffset = 1.0;
	}
	else if (BoxName.IsEqual(ATBSHUD::ScrollHitBoxS))
	{
		Pawn->MoveForwardAxisOffset = -1.0;
		Pawn->MoveRightAxisOffset = 0.0;
	}
	else if (BoxName.IsEqual(ATBSHUD::ScrollHitBoxSW))
	{
		Pawn->MoveForwardAxisOffset = -1.0;
		Pawn->MoveRightAxisOffset = -1.0;
	}
	else if (BoxName.IsEqual(ATBSHUD::ScrollHitBoxW))
	{
		Pawn->MoveForwardAxisOffset = 0.0;
		Pawn->MoveRightAxisOffset = -1.0;
	}
}

void ATBSHUD::NotifyHitBoxEndCursorOver(FName BoxName)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString(TEXT("End: ")).Append(BoxName.ToString()));
	Pawn->MoveForwardAxisOffset = 0.0;
	Pawn->MoveRightAxisOffset = 0.0;
}
