// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TBSClassLoader.h"
#include "TBSUIContextStack.h"

class TBSUIContextEvent;

/**
 * 
 */
class TBS_API TBSUIContext
{
public:
	TBSUIContext();
	~TBSUIContext();

	void Initialise(TBSUIContextStack* ContextStack, ATBSClassLoader* InClassLoader);

	virtual void HandleEvent(TBSUIContextEvent* Event);

protected:
	TBSUIContextStack* ContextStack;
	ATBSClassLoader* ClassLoader;
};
