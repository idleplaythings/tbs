// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Networking.h"
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
private:
	bool OnConnection(class FSocket* ClientSocket, const FIPv4Endpoint& ClientEndpoint);
	bool RecvMessage(FSocket *Socket, uint32 DataSize, char* Message);

	void ReadDataIntoMessageBuffer(char* Data, uint32 Length);
	bool InitMessageBuffer(char* Dataa, uint32 Length);

	TQueue<FSocket*> PendingClientSockets;
	TArray<FSocket*> ClientSockets;
	FTcpListener* SocketListener;

	char* MessageBuffer = nullptr;
	char* PartialBuffer = nullptr;

	uint32 DataOffset = 0;
	uint32 PartialLength = 0;
	uint32 PayloadLength = 0;
	uint32 BytesRead = 0;
};
