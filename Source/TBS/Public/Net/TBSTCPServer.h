// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Networking.h"
#include "TBSTCP.h"
#include "TcpListener.h"

/**
 * 
 */
class TBS_API TBSTCPServer : public TBSTCP
{
public:
	TBSTCPServer();
	~TBSTCPServer();

	virtual uint32 Run() override;

	bool Listen(FString IP, int32 Port);
	bool Send(uint32 ConnectionId, uint8_t Prefix, uint8_t* Message, uint32 Length, int32 &BytesSent);
	bool SendAll(uint8_t Prefix, uint8_t* Message, uint32 Length);

private:
	bool OnConnection(class FSocket* ClientSocket, const FIPv4Endpoint& ClientEndpoint);	

	uint32 NextConnectionId = 1;
	TQueue<FSocket*> PendingClientSockets;
	TMap<uint32, FSocket*> ClientSockets;
	FTcpListener* SocketListener;
};
