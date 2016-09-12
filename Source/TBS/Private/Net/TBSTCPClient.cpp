// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSTCPClient.h"

TBSTCPClient::TBSTCPClient()
{
	Thread = FRunnableThread::Create(this, TEXT("TBSTCPServer"), 8 * 1024, TPri_Normal);
}

TBSTCPClient::~TBSTCPClient()
{
	if (Socket)
	{
		Socket->Close();
	}

	if (Thread)
	{
		Thread->Kill(true);
		delete Thread;
	}
}

bool TBSTCPClient::Connect(FString IP, int32 Port)
{
	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

	TSharedRef<FInternetAddr> Address = SocketSubsystem->CreateInternetAddr();
	bool IsValidAddr = true;
	Address->SetIp(*IP, IsValidAddr);
	Address->SetPort(Port);
	
	if (IsValidAddr)
	{
		FString AddressStr = Address->ToString(true);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting to %s"), *AddressStr));

		Socket = SocketSubsystem->CreateSocket(NAME_Stream, FString(TEXT("Foo")), false);

		if (Socket)
		{
			//int32 BufferSize;			
			//if (Socket->SetSendBufferSize(4096, BufferSize))
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Send buffer size set, size %i"), BufferSize));
			//}
			//else
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Send buffer size not set, size %i"), BufferSize));
			//}

			Connected = Socket->Connect(*Address);
			if (Connected)
			{
				//char* Message = "Hello!\0";
				//SendMessage(Socket, Message, strlen(Message));
			}
			else
			{
				Stopping = true;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("NOT connected :(")));
			}
		}
	}

	return false;
}

bool TBSTCPClient::Send(const char* Payload)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TBSTCPClient::Send")));

	uint32 PayloadLength = strlen(Payload);
	uint8* PayloadLengthMsg = new uint8();
	memcpy(PayloadLengthMsg, &PayloadLength, sizeof(PayloadLength));

	int32 BytesSent = 0;
	if (Socket->Send(PayloadLengthMsg, sizeof(PayloadLength), BytesSent))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Length sent")));
	}

	if (Socket->Send((uint8*)Payload, PayloadLength, BytesSent))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Payload sent")));
	}

	return true;
	//SendMessage(Socket, PayloadLengthMsg, sizeof(PayloadLength));

	//if (Socket && Connected)
	//{
	//	SendMessage(Socket, Message, strlen(Message));
	//}
	//
	//return false;
}

bool TBSTCPClient::SendMessage(FSocket* Socket, const char* Message, int32 Length)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TBSTCPClient::SendMessage")));
	check(Socket);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Message length %i"), Length));

	int32 BytesSent = 0;
	bool Result = Socket->Send((uint8*)Message, Length, BytesSent);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Bytes sent: %i"), BytesSent));

	return Result;
}

//bool TBSTCPClient::RecvMessage(FSocket *Socket, uint32 DataSize, FString& Message)
//{
//	check(Socket);
//
//	FArrayReaderPtr Datagram = MakeShareable(new FArrayReader(true));
//	Datagram->Init(FMath::Min(DataSize, 65507u), 0);
//
//	int32 BytesRead = 0;
//	if (Socket->Recv(Datagram->GetData(), Datagram->Num(), BytesRead))
//	{
//		char* Data = (char*)Datagram->GetData();
//		Data[BytesRead] = '\0';
//		Message = UTF8_TO_TCHAR(Data);
//		return true;
//	}
//	return false;
//}

uint32 TBSTCPClient::Run()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TBSTCPServer::Run")));

	while (!Stopping)
	{
		if (Socket && Connected)
		{
			uint32 DataSize = 0;
			while (Socket->HasPendingData(DataSize))
			{
				//FString Request;
				//if (RecvMessage(Socket, DataSize, Request))
				//{
				//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some data here...")));
				//	/*FString Response = HandleClientMessage(Client, Request);
				//	SendMessage(Client, Response);*/
				//}
			}
		}
		FPlatformProcess::Sleep(0.2f);
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TBSTCPServer::Run end")));

	return 0;
}
