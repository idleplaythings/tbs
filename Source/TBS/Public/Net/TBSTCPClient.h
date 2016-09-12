// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Networking.h"
#include "TBSTCP.h"

/**
 * 
 */
class TBS_API TBSTCPClient : public TBSTCP
{
public:
	TBSTCPClient();
	~TBSTCPClient();

	virtual uint32 Run() override;

	bool Connect(FString IP, int32 Port);
	bool Send(const char* Message);

private:
	bool SendMessage(FSocket* Socket, const char* Message, int32 Length);
	//bool ReceiveMessage(FSocket* Socket, char* Message);
	//bool RecvMessage(FSocket *Socket, uint32 DataSize, FString& Message);

	FSocket* Socket;

	bool Connected = false;
};
