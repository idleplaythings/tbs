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

//bool TBSTCP::SendMessage(FSocket* Socket, uint8_t* Message, uint32 Length)
//{
//	int32 BytesSent = 0;
//	return SendMessage(Socket, Message, Length, BytesSent);
//}

bool TBSTCP::SendMessage(FSocket* Socket, uint8_t Prefix, uint8_t* Message, uint32 Length, int32 &BytesSent)
{
	uint32 TotalLength = Length + sizeof(uint8_t);

	if (!Socket->Send((uint8_t*)&TotalLength, sizeof(TotalLength), BytesSent))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Sending length failed")));
		return false;
	}
	
	if (!Socket->Send((uint8_t*)&Prefix, sizeof(uint8_t), BytesSent))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Sending prefix failed")));
		return false;
	}

	if (!Socket->Send((uint8_t*)Message, Length, BytesSent))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Sending payload failed")));
		return false;
	}
	
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
		TQueue<FNetworkMessage> ReceivedMessages;
		
		StreamReader->ReadData(Datagram->GetData(), BytesIn, ReceivedMessages);

		FNetworkMessage Message;
		int32 Messages = 0;

		while (ReceivedMessages.Dequeue(Message))
		{
			Messages++;
			if (ConnectionId > 0)
			{
				Message.ConnectionId = ConnectionId;
			}

			NetworkMessageQueue.Enqueue(Message);
		}

		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("%i Messages queued"), Messages));

		//for (auto& NetworkMessage : StreamReader->ReadData(Datagram->GetData(), BytesIn))
		//{
		//	if (ConnectionId > 0)
		//	{
		//		NetworkMessage.ConnectionId = ConnectionId;
		//	}
		//	
		//	NetworkMessageQueue.Enqueue(NetworkMessage);
		//}
		return true;
	}
	return false;
}
