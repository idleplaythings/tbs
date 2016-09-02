// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TBSClassLoader.h"

class TBSUIContextStack;
class TBSUIContextEvent;

/**
 * 
 */
class TBS_API TBSUIContext
{
public:
	TBSUIContext();
	~TBSUIContext();

	void Initialise(TBSUIContextStack* InContextStack, ATBSClassLoader* InClassLoader);

	virtual void EnterContext();
	virtual void ExitContext();
	virtual void HandleEvent(TBSUIContextEvent* Event);

protected:
	TBSUIContextStack* ContextStack;
	ATBSClassLoader* ClassLoader;
};
