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
	~TBSUIContextStack();

	void PushContext(TBSUIContext* Context);
	void PopContext();

	void HandleEvent(TBSUIContextEvent* Event);	

private:
	ATBSClassLoader* ClassLoader;
	std::vector<TBSUIContext*> UIContexts;
};
