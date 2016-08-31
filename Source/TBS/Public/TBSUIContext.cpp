// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSDefaultPawn.h"
#include "TBSUIContext.h"

TBSUIContext::TBSUIContext()
{
}

TBSUIContext::~TBSUIContext()
{
}

void TBSUIContext::Initialise(ATBSClassLoader* InClassLoader)
{
	ClassLoader = InClassLoader;
}

void TBSUIContext::HandleEvent(TBSUIContextEvent* Event)
{

}
//
//void TBSUIContext::OnTileClick(FIntVector)
//{
//
//}
//
//void TBSUIContext::OnTileRightClick(FIntVector)
//{
//
//}
//
//void TBSUIContext::OnTileHoverBegin(FIntVector)
//{
//
//}
//
//void TBSUIContext::OnTileHoverEnd(FIntVector)
//{
//
//}
//
//void TBSUIContext::OnMoveCameraForward(float AxisValue)
//{
//
//}
//
//void TBSUIContext::OnMoveCameraRight(float AxisValue)
//{
//
//}
//
//void TBSUIContext::OnLevelUp()
//{
//
//}
//
//void TBSUIContext::OnLevelDown()
//{
//
//}
//
//void TBSUIContext::OnRotateCameraRight()
//{
//
//}
//
//void TBSUIContext::OnRotateCameraLeft()
//{
//
//}
//
//void TBSUIContext::OnZoomIn()
//{
//
//}
//
//void TBSUIContext::OnZoomOut()
//{
//
//}
//
//void TBSUIContext::OnTogglePerspectiveCamera()
//{
//
//}
