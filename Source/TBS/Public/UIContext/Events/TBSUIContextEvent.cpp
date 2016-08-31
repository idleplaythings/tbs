// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSUIContextEvent.h"

TBSUIContextEvent::TBSUIContextEvent()
{
}

TBSUIContextEvent::TBSUIContextEvent(FName Type)
{
	this->Type = Type;
}

TBSUIContextEvent::~TBSUIContextEvent()
{
}
