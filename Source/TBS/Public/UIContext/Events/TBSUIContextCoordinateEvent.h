// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TBSUIContextEvent.h"

/**
 * 
 */
class TBS_API TBSUIContextCoordinateEvent : public TBSUIContextEvent
{
public:
	TBSUIContextCoordinateEvent();
	TBSUIContextCoordinateEvent(FName Type, FIntVector Coordinates);
	~TBSUIContextCoordinateEvent();

	FIntVector Coordinates;
};
