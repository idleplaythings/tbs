// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSUIContext.h"

TBSUIContext::TBSUIContext()
{
}

TBSUIContext::~TBSUIContext()
{
}

void TBSUIContext::Initialise(TBSUIContextStack* InContextStack, ATBSClassLoader* InClassLoader)
{
	ContextStack = InContextStack;
	ClassLoader = InClassLoader;
}

void TBSUIContext::EnterContext()
{

}

void TBSUIContext::ExitContext()
{

}

void TBSUIContext::HandleEvent(TBSUIContextEvent &Event)
{

}
