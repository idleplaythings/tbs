// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Networking.h"
#include "TBSStreamReader.h"

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

	TQueue<NetworkMessage> NetworkMessageQueue;

protected:
	bool SendMessage(FSocket* Socket, uint8_t* Message, uint32 Length);
	bool RecvMessage(FSocket *Socket, uint32 DataSize, uint32 ConnectionId=0);

	float ThreadSleepTime = 0.2f;
	uint32 MessageHeaderSize = 4;
	bool Stopping = true;
	FRunnableThread* Thread;
	TBSStreamReader* StreamReader;
};
