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
			Connected = Socket->Connect(*Address);
			if (!Connected)
			{
				Stopping = true;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("NOT connected :(")));
			}
		}
	}

	return Connected;
}

bool TBSTCPClient::Send(const char* Payload, uint32 Length)
{
	return SendMessage(Socket, Payload, Length);
}

uint32 TBSTCPClient::Run()
{
	while (!Stopping)
	{
		if (Socket && Connected)
		{
			uint32 DataSize = 0;
			while (Socket->HasPendingData(DataSize))
			{
				RecvMessage(Socket, DataSize);
			}
		}
		FPlatformProcess::Sleep(ThreadSleepTime);
	}

	return 0;
}
