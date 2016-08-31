// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class TBS_API TBSUIContextEvent
{
public:
	TBSUIContextEvent();
	TBSUIContextEvent(FName Type);
	~TBSUIContextEvent();

	FName Type;
};
