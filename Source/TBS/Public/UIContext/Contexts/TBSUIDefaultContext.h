// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TBSUIContext.h"

/**
 * 
 */
class TBS_API TBSUIDefaultContext : public TBSUIContext
{
public:
	void HandleEvent(TBSUIContextEvent* Event);

private:
	ATBSUnit* HoverUnit;
};
