// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSPlayerController.h"
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
	static ConstructorHelpers::FClassFinder<UTBSPropDebugWidget> WidgetAsset(TEXT("/Game/Blueprints/Widgets/PropTooltip.PropTooltip_C"));

	if (WidgetAsset.Succeeded())
	{
		/** Assign the class of the loaded asset to the WigetClass variable, which is a "subclass" of UUserWidget : Which our asset class is */
		PropDebugClass = WidgetAsset.Class;
	}
}

void ATBSHUD::BeginPlay()
{
	PlayerController = Cast<ATBSPlayerController>(GetOwningPlayerController());
}

void ATBSHUD::DrawHUD()
{
	Super::DrawHUD();
	CalculateScrollHitBoxSizesAndLocations();
	AddScrollHitBoxes();
	DrawScrollHitBoxOverlays();
}

void ATBSHUD::ShowPropDebugWidget(TArray<FProp> Props)
{
	if (!PropDebugClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString(TEXT("No prop debug class")));
		return;
	}

	if (!PropDebugWidget)
	{
		PropDebugWidget = CreateWidget<UTBSPropDebugWidget>(GetWorld(), PropDebugClass);
	}

	if (PropDebugWidget)
	{
		if (!PropDebugWidget->IsInViewport())
		{
			PropDebugWidget->AddToViewport();
		}
		
		PropDebugWidget->SetVisibility(ESlateVisibility::Visible);
		PropDebugWidget->SetProps(Props);
	}
}

void ATBSHUD::HidePropDebugWidget()
{
	if (PropDebugWidget)
	{
		PropDebugWidget->SetVisibility(ESlateVisibility::Hidden);
	}
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
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString(TEXT("Begin: ")).Append(BoxName.ToString()));

	if (!PlayerController)
	{
		return;
	}

	if (BoxName.IsEqual(ATBSHUD::ScrollHitBoxNW))
	{
		PlayerController->MoveCameraForwardOffset(1.0);
		PlayerController->MoveCameraRightOffset(-1.0);
	}
	else if (BoxName.IsEqual(ATBSHUD::ScrollHitBoxN))
	{
		PlayerController->MoveCameraForwardOffset(1.0);
		PlayerController->MoveCameraRightOffset(0.0);
	}
	else if (BoxName.IsEqual(ATBSHUD::ScrollHitBoxNE))
	{
		PlayerController->MoveCameraForwardOffset(1.0);
		PlayerController->MoveCameraRightOffset(1.0);
	}
	else if (BoxName.IsEqual(ATBSHUD::ScrollHitBoxE))
	{
		PlayerController->MoveCameraForwardOffset(0.0);
		PlayerController->MoveCameraRightOffset(1.0);
	}
	else if (BoxName.IsEqual(ATBSHUD::ScrollHitBoxSE))
	{
		PlayerController->MoveCameraForwardOffset(-1.0);
		PlayerController->MoveCameraRightOffset(1.0);
	}
	else if (BoxName.IsEqual(ATBSHUD::ScrollHitBoxS))
	{
		PlayerController->MoveCameraForwardOffset(-1.0);
		PlayerController->MoveCameraRightOffset(0.0);
	}
	else if (BoxName.IsEqual(ATBSHUD::ScrollHitBoxSW))
	{
		PlayerController->MoveCameraForwardOffset(-1.0);
		PlayerController->MoveCameraRightOffset(-1.0);
	}
	else if (BoxName.IsEqual(ATBSHUD::ScrollHitBoxW))
	{
		PlayerController->MoveCameraForwardOffset(0.0);
		PlayerController->MoveCameraRightOffset(-1.0);
	}
}

void ATBSHUD::NotifyHitBoxEndCursorOver(FName BoxName)
{
	if (!PlayerController)
	{
		return;
	}

	PlayerController->MoveCameraForwardOffset(0.0);
	PlayerController->MoveCameraRightOffset(0.0);
}
