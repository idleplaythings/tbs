// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "TBSGridUI.h"
#include "TBSPropDebugWidget.h"
#include "TBSHUD.generated.h"

class ATBSPlayerController;

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

public:
	ATBSHUD(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;
	virtual void DrawHUD() override;
	virtual void NotifyHitBoxBeginCursorOver(FName BoxName);
	virtual void NotifyHitBoxEndCursorOver(FName BoxName);

	static const FName ScrollHitBoxNW;
	static const FName ScrollHitBoxN;
	static const FName ScrollHitBoxNE;
	static const FName ScrollHitBoxE;
	static const FName ScrollHitBoxSE;
	static const FName ScrollHitBoxS;
	static const FName ScrollHitBoxSW;
	static const FName ScrollHitBoxW;

	void ShowPropDebugWidget(TArray<FProp> Props);
	void HidePropDebugWidget();

private:
	float HitBoxWidth = 20.0;
	bool ConsumeScrollInput = true;
	int32 ScrollHitBoxPriority = 0;
	bool DrawScrollOverlay = false;
	TArray<HitBox> HitBoxes;
	int32 CanvasX;
	int32 CanvasY;
	ATBSPlayerController* PlayerController;

	TSubclassOf<class UTBSPropDebugWidget> PropDebugClass;
	UTBSPropDebugWidget* PropDebugWidget;

	void CalculateScrollHitBoxSizesAndLocations();
	void AddScrollHitBoxes();
	void DrawScrollHitBoxOverlays();
};
