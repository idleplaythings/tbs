// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSUIContextEvent.h"
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
	Context->EnterContext();
}

void TBSUIContextStack::PopContext()
{
	UIContexts.back()->ExitContext();
	UIContexts.pop_back();
}

void TBSUIContextStack::HandleEvent(TBSUIContextEvent &Event)
{
	if (UIContexts.size() > 0)
	{
		// The contexts may mutate the context vector, so operate on a copy
		std::vector<TBSUIContext*> CopyOfContexts = UIContexts;

		for (std::vector<TBSUIContext*>::reverse_iterator It = CopyOfContexts.rbegin(); It != CopyOfContexts.rend(); ++It)
		{
			if (Event.StopPropagation)
			{
				break;
			}

			if (*It)
			{
				(*It)->HandleEvent(Event);
			}
		}
	}
}
