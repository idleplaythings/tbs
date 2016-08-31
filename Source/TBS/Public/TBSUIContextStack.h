// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "TBS.h"
#include "TBSClassLoader.h"
#include "TBSUIContext.h"

class TBSUIContextEvent;

/**
 * 
 */
class TBS_API TBSUIContextStack
{
public:
	TBSUIContextStack(ATBSClassLoader* ClassLoader);

	void PushContext(TBSUIContext* Context);
	void PopContext();

	void HandleEvent(TBSUIContextEvent* Event);

	//void OnTileClick(FIntVector);
	//void OnTileRightClick(FIntVector);
	//void OnTileHoverBegin(FIntVector);
	//void OnTileHoverEnd(FIntVector);
	//void OnMoveCameraForward(float AxisValue);
	//void OnMoveCameraRight(float AxisValue);
	//void OnLevelUp();
	//void OnLevelDown();
	//void OnRotateCameraRight();
	//void OnRotateCameraLeft();
	//void OnZoomIn();
	//void OnZoomOut();
	//void OnTogglePerspectiveCamera();

	~TBSUIContextStack();

private:
	ATBSClassLoader* ClassLoader;
	std::vector<TBSUIContext*> UIContexts;
};
