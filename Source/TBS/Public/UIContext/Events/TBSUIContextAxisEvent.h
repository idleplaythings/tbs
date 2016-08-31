// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TBSUIContextEvent.h"

/**
 * 
 */
class TBS_API TBSUIContextAxisEvent : public TBSUIContextEvent
{
public:
	TBSUIContextAxisEvent();
	TBSUIContextAxisEvent(FName Type, float AxisValue);
	~TBSUIContextAxisEvent();

	float AxisValue;
};
