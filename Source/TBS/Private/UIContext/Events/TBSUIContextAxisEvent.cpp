// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSUIContextAxisEvent.h"

TBSUIContextAxisEvent::TBSUIContextAxisEvent()
{
}

TBSUIContextAxisEvent::TBSUIContextAxisEvent(FName Type, float AxisValue)
{
	this->Type = Type;
	this->AxisValue = AxisValue;
}

TBSUIContextAxisEvent::~TBSUIContextAxisEvent()
{
}
