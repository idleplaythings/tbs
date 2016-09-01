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
	UIContexts.push_back((TBSUIContext*) Context);
}

void TBSUIContextStack::PopContext()
{
	UIContexts.pop_back();
}

void TBSUIContextStack::HandleEvent(TBSUIContextEvent* Event)
{
	// The contexts may mutate the context vector, so operate on a copy
	std::vector<TBSUIContext*> CopyOfContexts = UIContexts;

	for (std::vector<TBSUIContext*>::reverse_iterator It = CopyOfContexts.rbegin(); It != CopyOfContexts.rend(); ++It)
	{
		if (*It)
		{
			(*It)->HandleEvent(Event);
		}		
	}
}
