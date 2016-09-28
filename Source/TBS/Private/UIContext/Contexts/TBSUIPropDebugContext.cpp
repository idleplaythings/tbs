// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSUIContextEvent.h"
#include "TBSUIPropDebugContext.h"

void TBSUIPropDebugContext::EnterContext()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Prop debug")));

	//ATBSUnit* Unit = ClassLoader->PlayerController->SelectedUnit;

	//ClassLoader->GridUI->SelectLocation(Unit->Dimensions, Unit->GameCoordinates);
	//ClassLoader->GridUI->SetCursorDimensions(ClassLoader->PlayerController->SelectedUnit->Dimensions);
	//ClassLoader->GridPathFinder->BuildCache(Unit->GameCoordinates, Unit->Dimensions);
}

void TBSUIPropDebugContext::ExitContext()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Exit prop debug")));

	//ClassLoader->GridUI->ClearSelection();
	//ClassLoader->GridUI->ResetCursorDimensions();
	//ClassLoader->GridPathFinder->InvalidateCache();
}

void TBSUIPropDebugContext::HandleEvent(TBSUIContextEvent &Event)
{
	if (Event.Type == FName(TEXT("PropDebug")))
	{
		Event.StopPropagation = true;
		ContextStack->PopContext();
	}

	//if (Event.Type == FName(TEXT("TileHoverBegin")))
	//{
	//	if (ClassLoader->PlayerController->SelectedUnit)
	//	{
	//		ClassLoader->PlayerController->PathSelected = true;

	//		if (ClassLoader->GridPathFinder->FindPath(
	//			ClassLoader->PlayerController->SelectedUnit->GameCoordinates,
	//			((TBSUIContextCoordinateEvent*)Event)->Coordinates,
	//			ClassLoader->PlayerController->SelectedPath,
	//			ClassLoader->PlayerController->SelectedUnit->Dimensions
	//		))
	//		{
	//			ClassLoader->GridPathRenderer->RenderPath(ClassLoader->PlayerController->SelectedPath);
	//		}
	//		else
	//		{
	//			ClassLoader->GridPathRenderer->ClearPath();
	//		}
	//	}
	//}
	//else if (Event.Type == FName(TEXT("TileHoverEnd")))
	//{
	//	ClassLoader->GridPathRenderer->ClearPath();
	//}
	//else if (Event.Type == FName(TEXT("TileClick")))
	//{
	//	ClassLoader->PlayerController->Server_CommandMoveUnit(
	//		ClassLoader->PlayerController->SelectedUnit,
	//		ClassLoader->PlayerController->SelectedPath
	//	);

	//	ClassLoader->PlayerController->PathSelected = false;
	//	ClassLoader->GridPathRenderer->ClearPath();
	//	ContextStack->PopContext();
	//}
	//else if (Event.Type == FName(TEXT("TileRightClick")) || Event.Type == FName(TEXT("Escape")))
	//{
	//	ClassLoader->PlayerController->PathSelected = false;
	//	ClassLoader->GridPathRenderer->ClearPath();
	//	ContextStack->PopContext();
	//}
}