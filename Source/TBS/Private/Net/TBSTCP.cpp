// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSTCP.h"

TBSTCP::TBSTCP()
{
	StreamReader = new TBSStreamReader(MessageHeaderSize);
}

TBSTCP::~TBSTCP()
{
}


bool TBSTCP::Init()
{
	Stopping = false;
	return true;
}

void TBSTCP::Stop()
{
	Stopping = true;
}

uint32 TBSTCP::Run()
{
	return 0;
}

bool TBSTCP::SendMessage(FSocket* Socket, uint8_t* Message, uint32 Length)
{
	int32 BytesSent = 0;
	Socket->Send((uint8_t*)&Length, sizeof(Length), BytesSent);
	Socket->Send((uint8_t*)Message, Length, BytesSent);

	return true;
}

bool TBSTCP::RecvMessage(FSocket *Socket, uint32 DataSize, uint32 ConnectionId)
{
	check(Socket);

	FArrayReaderPtr Datagram = MakeShareable(new FArrayReader(true));
	Datagram->Init(0, FMath::Min(DataSize, 65507u));

	int32 BytesIn = 0;
	if (Socket->Recv(Datagram->GetData(), Datagram->Num(), BytesIn))
	{
		for (auto& NetworkMessage : StreamReader->ReadData(Datagram->GetData(), BytesIn))
		{
			if (ConnectionId > 0)
			{
				NetworkMessage.ConnectionId = ConnectionId;
			}
			
			NetworkMessageQueue.Enqueue(NetworkMessage);
		}
		return true;
	}
	return false;
}
