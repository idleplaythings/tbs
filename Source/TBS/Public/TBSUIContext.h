// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TBSClassLoader.h"

class TBSUIContextEvent;

/**
 * 
 */
class TBS_API TBSUIContext
{
public:
	TBSUIContext();
	~TBSUIContext();

	void Initialise(ATBSClassLoader* InClassLoader);

	virtual void HandleEvent(TBSUIContextEvent* Event);

	//virtual void OnTileClick(FIntVector);
	//virtual void OnTileRightClick(FIntVector);
	//virtual void OnTileHoverBegin(FIntVector);
	//virtual void OnTileHoverEnd(FIntVector);
	//virtual void OnMoveCameraForward(float AxisValue);
	//virtual void OnMoveCameraRight(float AxisValue);
	//virtual void OnLevelUp();
	//virtual void OnLevelDown();
	//virtual void OnRotateCameraRight();
	//virtual void OnRotateCameraLeft();
	//virtual void OnZoomIn();
	//virtual void OnZoomOut();
	//virtual void OnTogglePerspectiveCamera();


protected:
	ATBSClassLoader* ClassLoader;
};
