// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include <string.h>
#include "TBSTCPServer.h"

TBSTCPServer::TBSTCPServer() :
	SocketListener(nullptr),
	MessageBuffer(nullptr),
	PartialBuffer(nullptr),
	DataOffset(0),
	PartialLength(0),
	PayloadLength(0),
	BytesRead(0)
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

	for (auto& ClientSocket : ClientSockets)
	{
		ClientSocket->Close();
	}

	if (Thread)
	{
		Thread->Kill(true);
		delete Thread;
	}
}

bool TBSTCPServer::Listen(FString IP, int32 Port)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TBSTCPServer::Listen")));

	if (!SocketListener)
	{		
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No socket listener")));

		TSharedRef<FInternetAddr> Address = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

		bool IsValidAddr = true;
		Address->SetIp(*IP, IsValidAddr);
		Address->SetPort(Port);

		FString AddressStr = Address->ToString(true);

		if (!IsValidAddr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Invalid address %s"), *AddressStr));

			return false;
		}
		
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Listening on %s"), *AddressStr));

		SocketListener = new FTcpListener(FIPv4Endpoint(Address), FTimespan(0, 0, 1));
		SocketListener->OnConnectionAccepted().BindRaw(this, &TBSTCPServer::OnConnection);
	}

	return true;
}

void TBSTCPServer::ReadDataIntoMessageBuffer(char* Data, uint32 Length)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Reading into message buffer %i bytes"), Length));

	if (MessageBuffer == nullptr)
	{
		if (InitMessageBuffer(Data, Length))
		{
			//PartialBuffer = nullptr;
			//free(PartialBuffer);
			delete PartialBuffer;
			PartialBuffer = nullptr;
			PartialLength = 0;
		}
		else
		{
			return;
		}
	}

	uint32 BytesToRead = PayloadLength - BytesRead;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Bytes to read %i"), BytesToRead));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Actual length %i"), Length - DataOffset));

	BytesToRead = BytesToRead > (Length - DataOffset) ? (Length - DataOffset) : BytesToRead;
	//BytesToRead = Length - DataOffset;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Corrected bytes to read %i"), BytesToRead));

	/*strcpy_s(MessageBuffer + BytesRead, PayloadLength + 1, Data + DataOffset);*/
	memcpy(MessageBuffer + BytesRead, Data + DataOffset, BytesToRead);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("A %i"), BytesRead));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("B %i"), BytesToRead));

	BytesRead = BytesRead + BytesToRead;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Bytes read %i"), BytesRead));

	if (BytesRead == PayloadLength)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Queue is empty %i"), (NetworkMessageQueue.IsEmpty() ? 1 : 0)));

		char* Fixed = new char[PayloadLength + 1];
		memcpy(Fixed, MessageBuffer, PayloadLength);
		Fixed[PayloadLength] = '\0';
		NetworkMessageQueue.Enqueue(ANSI_TO_TCHAR(Fixed));

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Message of length %i read!"), BytesRead));		

		//MessageQueue.Enqueue(ANSI_TO_TCHAR(MessageBuffer));
		//NetworkMessageQueue.Enqueue()

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Length %i, Data offset %i, bytes to read %i"), Length, DataOffset, BytesToRead));

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Message %s"), ANSI_TO_TCHAR(MessageBuffer)));

		//OnNetworkMessage.Broadcast(ANSI_TO_TCHAR(MessageBuffer));

		//free(MessageBuffer);
		//free(PartialBuffer);
		delete MessageBuffer;
		MessageBuffer = nullptr;
		delete PartialBuffer;
		PartialBuffer = nullptr;
		
		char* NewStart = Data + DataOffset + BytesToRead;
		uint32 NewLength = Length - DataOffset - BytesToRead;

		PartialLength = 0;
		PayloadLength = 0;
		BytesRead = 0;
		DataOffset = 0;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("New length %i"), NewLength));

		if (NewLength > 0)
		{
			ReadDataIntoMessageBuffer(NewStart, NewLength);
		}
	}

	DataOffset = 0;
}

bool TBSTCPServer::InitMessageBuffer(char* Data, uint32 Length)
{	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Initialising message buffer")));

	if (PartialLength > 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Partial buffer")));

		if (PartialLength + Length > 4)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Message buffer now done")));
			
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Remaining buffer %i"), 4 - PartialLength));

			//strcpy_s(PartialBuffer + PartialLength, 4 - PartialLength, Data);
			memcpy(PartialBuffer + PartialLength, Data, 4 - PartialLength);
			PayloadLength = *(reinterpret_cast<uint32 *>(PartialBuffer));

			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Payload length %i"), PayloadLength));

			MessageBuffer = new char[PayloadLength];
			//strcpy_s(MessageBuffer, Length - (4 - PartialLength), Data + (4 - PartialLength));
			memcpy(MessageBuffer, Data + (4 - PartialLength), Length - (4 - PartialLength));
			//BytesRead = Length - (4 - PartialLength);
			BytesRead = 0;
			DataOffset = 4 - PartialLength;

			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Payload length %i"), PayloadLength));

			//free(PartialBuffer);
			delete PartialBuffer;
			PartialBuffer = nullptr;
			PartialLength = 0;

			return true;
		}

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Another partial")));

		/*strcpy_s(PartialBuffer + PartialLength, Length, Data);*/
		memcpy(PartialBuffer + PartialLength, Data, Length);
		PartialLength += Length;
		return false;
	}

	if (Length < 4)
	{
		PartialBuffer = new char[4];
		//strcpy(PartialBuffer, Data);
		memcpy(PartialBuffer, Data, Length);
		PartialLength = Length;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Not enough to complete message buffer, partial length %i"), PartialLength));

		return false;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Message buffer now done2")));

	PayloadLength = *(reinterpret_cast<uint32 *>(Data));

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Payload length2 %i"), PayloadLength));

	MessageBuffer = new char[PayloadLength];
	BytesRead = 0;
	DataOffset = 4;
	return true;
}

bool TBSTCPServer::RecvMessage(FSocket *Socket, uint32 DataSize, char* Message)
{
	check(Socket);

	FArrayReaderPtr Datagram = MakeShareable(new FArrayReader(true));
	Datagram->Init(0, FMath::Min(DataSize, 65507u));

	int32 BytesIn = 0;
	if (Socket->Recv(Datagram->GetData(), Datagram->Num(), BytesIn))
	{
		char* Data = (char*)Datagram->GetData();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Data array length %i, bytes read %i"), strlen(Data), BytesIn));
		ReadDataIntoMessageBuffer(Data, BytesIn);

		return true;
	}
	return false;
}

uint32 TBSTCPServer::Run()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TBSTCPServer::Run")));

	while (!Stopping)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Pending sockets empty %i, client sockets %i"), (PendingClientSockets.IsEmpty() ? 1 : 0), ClientSockets.Num()));

		if (!PendingClientSockets.IsEmpty())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Not empty")));

			FSocket *Client = NULL;
			while (PendingClientSockets.Dequeue(Client))
			{
				ClientSockets.Add(Client);
			}
		}

		// remove closed connections
		for (int32 ClientIndex = ClientSockets.Num() - 1; ClientIndex >= 0; --ClientIndex)
		{
			if (ClientSockets[ClientIndex]->GetConnectionState() != SCS_Connected)
			{
				ClientSockets.RemoveAtSwap(ClientIndex);
			}
		}

		// Poll data from every connected client
		for (TArray<class FSocket*>::TIterator ClientIt(ClientSockets); ClientIt; ++ClientIt)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Looping clients")));

			FSocket *Client = *ClientIt;
			uint32 DataSize = 0;
			while (Client->HasPendingData(DataSize))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("!!!!!!!!!!! RECEIVING MESSAGE")));

				char* Message = new char();
				if (RecvMessage(Client, DataSize, Message))
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Received length: %i"), DataSize));

					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some data here: %s"), *Message));

					/*FString Response = HandleClientMessage(Client, Request);
					SendMessage(Client, Response);*/
				}
			}
		}

		FPlatformProcess::Sleep(0.2f);
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TBSTCPServer::Run end")));

	return 0;
}

bool TBSTCPServer::OnConnection(FSocket * ClientSocket, const FIPv4Endpoint & ClientEndpoint)
{	
	if (PendingClientSockets.Enqueue(ClientSocket))
	{
		//int32 BufferSize;
		//if (ClientSocket->SetReceiveBufferSize(4096, BufferSize))
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv buffer size set, size %i"), BufferSize));
		//}
		//else
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv buffer size not set, size %i"), BufferSize));
		//}

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Client socket queued...")));
	}

	return true;
}
