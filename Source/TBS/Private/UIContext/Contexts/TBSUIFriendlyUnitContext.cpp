// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSUIContextEvent.h"
#include "TBSUIContextCoordinateEvent.h"
#include "TBSClassLoader.h"
#include "TBSPlayerController.h"
#include "TBSUIFriendlyUnitContext.h"

void TBSUIFriendlyUnitContext::EnterContext()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Unit's occupied coordinates")));
	//FString Coordinates;

	//for (auto& Coordinate : ClassLoader->PlayerController->SelectedUnit->GameCoordinatesOccupied)
	//{
	//	Coordinates.Append(FString::Printf(TEXT("(%i, %i, %i) "), Coordinate.X, Coordinate.Y, Coordinate.Z));
	//}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Coordinates);

	ClassLoader->GridUI->SelectCoordinates(ClassLoader->PlayerController->SelectedUnit->GameCoordinatesOccupied);
}

void TBSUIFriendlyUnitContext::ExitContext()
{
	ClassLoader->GridUI->ClearSelection();
}

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
