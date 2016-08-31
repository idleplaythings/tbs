// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSUIContextStack.h"

TBSUIContextStack::TBSUIContextStack(ATBSClassLoader* ClassLoader) : ClassLoader(ClassLoader)
{
}

TBSUIContextStack::~TBSUIContextStack()
{
}

void TBSUIContextStack::PushContext(TBSUIContext* Context)
{
	Context->Initialise(ClassLoader);
	UIContexts.push_back(Context);
}

void TBSUIContextStack::PopContext()
{
	UIContexts.pop_back();
}

void TBSUIContextStack::HandleEvent(TBSUIContextEvent* Event)
{
	for (std::vector<TBSUIContext*>::iterator It = UIContexts.begin(); It != UIContexts.end(); ++It)
	{
		(*It)->HandleEvent(Event);
	}
}
//
//void TBSUIContextStack::OnTileClick(FIntVector Coordinates)
//{
//	for (std::vector<TBSUIContext*>::iterator It = UIContexts.begin(); It != UIContexts.end(); ++It)
//	{
//		(*It)->OnTileClick(Coordinates);
//	}
//}
//
//void TBSUIContextStack::OnTileRightClick(FIntVector Coordinates)
//{
//	for (std::vector<TBSUIContext*>::iterator It = UIContexts.begin(); It != UIContexts.end(); ++It)
//	{
//		(*It)->OnTileRightClick(Coordinates);
//	}
//}
//
//void TBSUIContextStack::OnTileHoverBegin(FIntVector Coordinates)
//{
//	for (std::vector<TBSUIContext*>::iterator It = UIContexts.begin(); It != UIContexts.end(); ++It)
//	{
//		(*It)->OnTileHoverBegin(Coordinates);
//	}
//}
//
//void TBSUIContextStack::OnTileHoverEnd(FIntVector Coordinates)
//{
//	for (std::vector<TBSUIContext*>::iterator It = UIContexts.begin(); It != UIContexts.end(); ++It)
//	{
//		(*It)->OnTileHoverEnd(Coordinates);
//	}
//}
//
//void TBSUIContextStack::OnMoveCameraForward(float AxisValue)
//{
//	for (std::vector<TBSUIContext*>::iterator It = UIContexts.begin(); It != UIContexts.end(); ++It)
//	{
//		(*It)->OnMoveCameraForward(AxisValue);
//	}
//}
//
//void TBSUIContextStack::OnMoveCameraRight(float AxisValue)
//{
//	for (std::vector<TBSUIContext*>::iterator It = UIContexts.begin(); It != UIContexts.end(); ++It)
//	{
//		(*It)->OnMoveCameraRight(AxisValue);
//	}
//}
//
//void TBSUIContextStack::OnLevelUp()
//{
//	for (std::vector<TBSUIContext*>::iterator It = UIContexts.begin(); It != UIContexts.end(); ++It)
//	{
//		(*It)->OnLevelUp();
//	}
//}
//
//void TBSUIContextStack::OnLevelDown()
//{
//	for (std::vector<TBSUIContext*>::iterator It = UIContexts.begin(); It != UIContexts.end(); ++It)
//	{
//		(*It)->OnLevelDown();
//	}
//}
//
//void TBSUIContextStack::OnRotateCameraRight()
//{
//	for (std::vector<TBSUIContext*>::iterator It = UIContexts.begin(); It != UIContexts.end(); ++It)
//	{
//		(*It)->OnRotateCameraRight();
//	}
//}
//
//void TBSUIContextStack::OnRotateCameraLeft()
//{
//	for (std::vector<TBSUIContext*>::iterator It = UIContexts.begin(); It != UIContexts.end(); ++It)
//	{
//		(*It)->OnRotateCameraLeft();
//	}
//}
//
//void TBSUIContextStack::OnZoomIn()
//{
//	for (std::vector<TBSUIContext*>::iterator It = UIContexts.begin(); It != UIContexts.end(); ++It)
//	{
//		(*It)->OnZoomIn();
//	}
//}
//
//void TBSUIContextStack::OnZoomOut()
//{
//	for (std::vector<TBSUIContext*>::iterator It = UIContexts.begin(); It != UIContexts.end(); ++It)
//	{
//		(*It)->OnZoomOut();
//	}
//}
//
//void TBSUIContextStack::OnTogglePerspectiveCamera()
//{
//	for (std::vector<TBSUIContext*>::iterator It = UIContexts.begin(); It != UIContexts.end(); ++It)
//	{
//		(*It)->OnTogglePerspectiveCamera();
//	}
//}

