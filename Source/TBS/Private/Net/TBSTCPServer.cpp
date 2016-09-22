// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include <string.h>
#include "TBSTCPServer.h"

TBSTCPServer::TBSTCPServer() :
	SocketListener(nullptr)
{
	Thread = FRunnableThread::Create(this, TEXT("TBSTCPServer"), 8 * 1024, TPri_Normal);
}

TBSTCPServer::~TBSTCPServer()
{
	if (SocketListener)
	{
		SocketListener->Stop();
		delete SocketListener;
		SocketListener = nullptr;
	}

	if (!PendingClientSockets.IsEmpty())
	{
		FSocket* Client;
		while (PendingClientSockets.Dequeue(Client))
		{
			Client->Close();
		}
	}

	for (auto& It : ClientSockets)
	{
		(*It.Value).Close();
	}

	if (Thread)
	{
		Thread->Kill(true);
		delete Thread;
	}
}

bool TBSTCPServer::Listen(FString IP, int32 Port)
{
	if (!SocketListener)
	{		
		TSharedRef<FInternetAddr> Address = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

		bool IsValidAddr = true;
		Address->SetIp(*IP, IsValidAddr);
		Address->SetPort(Port);

		FString AddressStr = Address->ToString(true);

		if (!IsValidAddr)
		{
			return false;
		}

		SocketListener = new FTcpListener(FIPv4Endpoint(Address), FTimespan(0, 0, 1));
		SocketListener->OnConnectionAccepted().BindRaw(this, &TBSTCPServer::OnConnection);
	}

	return true;
}

uint32 TBSTCPServer::Run()
{
	while (!Stopping)
	{
		if (!PendingClientSockets.IsEmpty())
		{
			FSocket *Client = NULL;
			while (PendingClientSockets.Dequeue(Client))
			{
				ClientSockets.Add(NextConnectionId++, Client);
			}
		}

		for (auto& It : ClientSockets)
		{
			FSocket *Socket = It.Value;

			if (Socket->GetConnectionState() != SCS_Connected)
			{
				ClientSockets.Remove(It.Key);
				continue;
			}

			uint32 DataSize = 0;
			while (Socket->HasPendingData(DataSize))
			{
				RecvMessage(Socket, DataSize, It.Key);
			}
		}

		FPlatformProcess::Sleep(0.2f);
	}

	return 0;
}


bool TBSTCPServer::OnConnection(FSocket * ClientSocket, const FIPv4Endpoint & ClientEndpoint)
{	
	return PendingClientSockets.Enqueue(ClientSocket);
}

bool TBSTCPServer::Send(uint32 ConnectionId, uint8_t* Payload, uint32 Length)
{
	FSocket* Socket = *ClientSockets.Find(ConnectionId);

	if (Socket)
	{
		return SendMessage(Socket, Payload, Length);
	}

	return false;
}

bool TBSTCPServer::SendAll(uint8_t* Payload, uint32 Length)
{
	bool Result = true;

	for (auto& It : ClientSockets)
	{
		FSocket *Socket = It.Value;

		if (Socket)
		{
			if (!SendMessage(Socket, Payload, Length))
			{
				Result = false;
			}
		}
	}

	return Result;
}


