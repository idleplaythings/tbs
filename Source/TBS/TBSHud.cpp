// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSHUD.h"

ATBSHUD::ATBSHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
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
	TopLeft.Name = FName(TEXT("ScrollHitBox,0"));
	HitBoxes.Add(TopLeft);

	TopCenter.Position = FVector2D(HitBoxWidth, 0);
	TopCenter.Size = FVector2D(CanvasX - 2 * HitBoxWidth, HitBoxWidth);
	TopCenter.Name = FName(TEXT("ScrollHitBox,1"));
	HitBoxes.Add(TopCenter);

	TopRight.Position = FVector2D(CanvasX - HitBoxWidth, 0);
	TopRight.Size = TopLeft.Size;
	TopRight.Name = FName(TEXT("ScrollHitBox,2"));
	HitBoxes.Add(TopRight);

	MiddleRight.Position = FVector2D(CanvasX - HitBoxWidth, HitBoxWidth);
	MiddleRight.Size = FVector2D(HitBoxWidth, CanvasY - 2 * HitBoxWidth);
	MiddleRight.Name = FName(TEXT("ScrollHitBox,4"));
	HitBoxes.Add(MiddleRight);

	BottomRight.Position = FVector2D(CanvasX - HitBoxWidth, CanvasY - HitBoxWidth);
	BottomRight.Size = TopLeft.Size;
	BottomRight.Name = FName(TEXT("ScrollHitBox,5"));
	HitBoxes.Add(BottomRight);

	BottomCenter.Position = FVector2D(HitBoxWidth, CanvasY - HitBoxWidth);
	BottomCenter.Size = TopCenter.Size;
	BottomCenter.Name = FName(TEXT("ScrollHitBox,6"));
	HitBoxes.Add(BottomCenter);

	BottomLeft.Position = FVector2D(0, CanvasY - HitBoxWidth);
	BottomLeft.Size = TopLeft.Size;
	BottomLeft.Name = FName(TEXT("ScrollHitBox,7"));
	HitBoxes.Add(BottomLeft);

	MiddleLeft.Position = FVector2D(0, HitBoxWidth);
	MiddleLeft.Size = MiddleRight.Size;
	MiddleLeft.Name = FName(TEXT("ScrollHitBox,8"));
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
