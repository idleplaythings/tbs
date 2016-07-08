// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "TBSDefaultPawn.h"
#include "TBSHUD.generated.h"

struct HitBox
{
	FVector2D Position;
	FVector2D Size;
	FName Name;
};

/**
 * 
 */
UCLASS()
class TBS_API ATBSHUD : public AHUD
{
	GENERATED_BODY()

	ATBSHUD(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;
	virtual void DrawHUD() override;
	virtual void NotifyHitBoxBeginCursorOver(FName BoxName);
	virtual void NotifyHitBoxEndCursorOver(FName BoxName);
	
	ATBSDefaultPawn* Pawn;

	static const FName ScrollHitBoxNW;
	static const FName ScrollHitBoxN;
	static const FName ScrollHitBoxNE;
	static const FName ScrollHitBoxE;
	static const FName ScrollHitBoxSE;
	static const FName ScrollHitBoxS;
	static const FName ScrollHitBoxSW;
	static const FName ScrollHitBoxW;

private:
	float HitBoxWidth = 20.0;
	bool ConsumeScrollInput = true;
	int32 ScrollHitBoxPriority = 0;
	bool DrawScrollOverlay = true;
	TArray<HitBox> HitBoxes;
	int32 CanvasX;
	int32 CanvasY;

	void CalculateScrollHitBoxSizesAndLocations();
	void AddScrollHitBoxes();
	void DrawScrollHitBoxOverlays();
};
