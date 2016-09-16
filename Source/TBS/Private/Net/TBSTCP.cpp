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

bool TBSTCP::SendMessage(FSocket* Socket, const char* Message, uint32 Length)
{
	uint8* PayloadLengthMsg = new uint8();
	memcpy(PayloadLengthMsg, &Length, sizeof(Length));

	int32 BytesSent = 0;
	Socket->Send(PayloadLengthMsg, sizeof(Length), BytesSent);
	Socket->Send((uint8*)Message, Length, BytesSent);

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
		for (auto& NetworkMessage : StreamReader->ReadData((char*)Datagram->GetData(), BytesIn))
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
