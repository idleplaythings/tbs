// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Networking.h"

/**
 * 
 */
class TBS_API TBSTCP : public FRunnable
{
public:
	TBSTCP();
	~TBSTCP();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override { }

	TQueue<FString> NetworkMessageQueue;

protected:
	bool Stopping = true;
	FRunnableThread* Thread;
};
