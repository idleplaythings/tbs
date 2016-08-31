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
	Context->Initialise(this, ClassLoader);
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
