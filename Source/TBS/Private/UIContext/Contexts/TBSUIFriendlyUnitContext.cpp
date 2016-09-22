// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSUIContextEvent.h"
#include "TBSUIContextCoordinateEvent.h"
#include "TBSClassLoader.h"
#include "TBSPlayerController.h"
#include "TBSUIFriendlyUnitContext.h"

void TBSUIFriendlyUnitContext::EnterContext()
{
	ATBSUnit* Unit = ClassLoader->PlayerController->SelectedUnit;

	ClassLoader->GridUI->SelectLocation(Unit->Dimensions, Unit->GameCoordinates);
	ClassLoader->GridUI->SetCursorDimensions(ClassLoader->PlayerController->SelectedUnit->Dimensions);
	ClassLoader->GridPathFinder->BuildCache(Unit->GameCoordinates, Unit->Dimensions);
}

void TBSUIFriendlyUnitContext::ExitContext()
{
	ClassLoader->GridUI->ClearSelection();
	ClassLoader->GridUI->ResetCursorDimensions();
	ClassLoader->GridPathFinder->InvalidateCache();
}

void TBSUIFriendlyUnitContext::HandleEvent(TBSUIContextEvent* Event)
{
	if (Event->Type == FName(TEXT("TileHoverBegin")))
	{
		if (ClassLoader->PlayerController->SelectedUnit)
		{
			ClassLoader->PlayerController->PathSelected = true;

			if (ClassLoader->GridPathFinder->FindPath(
				ClassLoader->PlayerController->SelectedUnit->GameCoordinates,
				((TBSUIContextCoordinateEvent*)Event)->Coordinates,
				ClassLoader->PlayerController->SelectedPath,
				ClassLoader->PlayerController->SelectedUnit->Dimensions
			))
			{
				ClassLoader->GridPathRenderer->RenderPath(ClassLoader->PlayerController->SelectedPath);
			}			
			else
			{
				ClassLoader->GridPathRenderer->ClearPath();
			}
		}
	}
	else if (Event->Type == FName(TEXT("TileHoverEnd")))
	{
		ClassLoader->GridPathRenderer->ClearPath();
	}
	else if (Event->Type == FName(TEXT("TileClick")))
	{
		ClassLoader->PlayerController->Server_CommandMoveUnit(
			ClassLoader->PlayerController->SelectedUnit,
			ClassLoader->PlayerController->SelectedPath
		);

		ClassLoader->PlayerController->PathSelected = false;
		ClassLoader->GridPathRenderer->ClearPath();
		ContextStack->PopContext();
	}
	else if (Event->Type == FName(TEXT("TileRightClick")) || Event->Type == FName(TEXT("Escape")))
	{
		ClassLoader->PlayerController->PathSelected = false;
		ClassLoader->GridPathRenderer->ClearPath();
		ContextStack->PopContext();
	}
}
