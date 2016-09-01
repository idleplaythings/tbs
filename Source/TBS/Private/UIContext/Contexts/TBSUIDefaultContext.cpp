// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSDefaultPawn.h"
#include "TBSClassLoader.h"
#include "TBSPlayerController.h"
#include "TBSUIContextEvent.h"
#include "TBSUIContextAxisEvent.h"
#include "TBSUIContextCoordinateEvent.h"
#include "TBSUIFriendlyUnitContext.h"
#include "TBSUIDefaultContext.h"

void TBSUIDefaultContext::HandleEvent(TBSUIContextEvent* Event)
{
	if (Event->Type == FName(TEXT("TileClick")))
	{
		FIntVector Coordinates = ((TBSUIContextCoordinateEvent*)Event)->Coordinates;
		ATBSUnit* Unit = ClassLoader->Grid->SelectUnit(Coordinates);

		if (Unit && Unit->PlayerNumber == ClassLoader->PlayerController->PlayerNumber)
		{
			ClassLoader->PlayerController->SelectedUnit = Unit;
			ContextStack->PushContext(new TBSUIFriendlyUnitContext);
		}		
	}
	else if (Event->Type == FName(TEXT("TileRightClick")))
	{

	}
	else if (Event->Type == FName(TEXT("TileHoverBegin")))
	{

	}
	else if (Event->Type == FName(TEXT("TileHoverEnd")))
	{

	}
	else if (Event->Type == FName(TEXT("MoveCameraForward")))
	{
		ClassLoader->DefaultPawn->MoveCameraForward(((TBSUIContextAxisEvent*)Event)->AxisValue);
	}
	else if (Event->Type == FName(TEXT("MoveCameraRight")))
	{
		ClassLoader->DefaultPawn->MoveCameraRight(((TBSUIContextAxisEvent*)Event)->AxisValue);
	}
	else if (Event->Type == FName(TEXT("ViewLevelUp")))
	{
		ClassLoader->GridUI->LevelUp();
	}
	else if (Event->Type == FName(TEXT("ViewLevelDown")))
	{
		ClassLoader->GridUI->LevelDown();
	}
	else if (Event->Type == FName(TEXT("RotateCameraRight")))
	{
		ClassLoader->DefaultPawn->RotateCameraRight();
	}
	else if (Event->Type == FName(TEXT("RotateCameraLeft")))
	{
		ClassLoader->DefaultPawn->RotateCameraLeft();
	}
	else if (Event->Type == FName(TEXT("ZoomCameraIn")))
	{
		ClassLoader->DefaultPawn->ZoomCameraIn();
	}
	else if (Event->Type == FName(TEXT("ZoomCameraOut")))
	{
		ClassLoader->DefaultPawn->ZoomCameraOut();
	}
	else if (Event->Type == FName(TEXT("TogglePerspectiveCamera")))
	{
		ClassLoader->DefaultPawn->TogglePerspectiveCamera();
	}
}
