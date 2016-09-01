// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSUIContextEvent.h"
#include "TBSUIContextCoordinateEvent.h"
#include "TBSClassLoader.h"
#include "TBSPlayerController.h"
#include "TBSUIFriendlyUnitContext.h"


void TBSUIFriendlyUnitContext::HandleEvent(TBSUIContextEvent* Event)
{
	if (Event->Type == FName(TEXT("TileHoverBegin")))
	{
		if (ClassLoader->PlayerController->SelectedUnit)
		{
			ClassLoader->PlayerController->PathSelected = true;

			ClassLoader->GridPathFinder->FindPath(
				ClassLoader->PlayerController->SelectedUnit->GameCoordinates,
				((TBSUIContextCoordinateEvent*)Event)->Coordinates,
				ClassLoader->PlayerController->SelectedPath
			);

			ClassLoader->GridPathRenderer->RenderPath(ClassLoader->PlayerController->SelectedPath);
		}
	}
	else if (Event->Type == FName(TEXT("TileHoverEnd")))
	{
		ClassLoader->GridPathRenderer->ClearPath();
	}
	else if (Event->Type == FName(TEXT("TileClick")))
	{
		ClassLoader->PlayerController->Server_HandleCommand(
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
