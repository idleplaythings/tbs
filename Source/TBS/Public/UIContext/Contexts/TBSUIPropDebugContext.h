// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TBSUIContext.h"

/**
 * 
 */
class TBS_API TBSUIPropDebugContext : public TBSUIContext
{
public:
	void EnterContext();
	void ExitContext();
	void HandleEvent(TBSUIContextEvent &Event);
};
