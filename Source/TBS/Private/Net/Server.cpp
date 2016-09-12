// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "Server.h"


// Sets default values
AServer::AServer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

AServer::~AServer()
{
	if (Listener)
	{
		Listener->Stop();
		delete Listener;
		Listener = nullptr;
	}

	if (ClientSocket)
	{
		ClientSocket->Close();
	}

	if (ServerSocket)
	{
		ServerSocket->Close();
	}
}


// Called when the game starts or when spawned
void AServer::BeginPlay()
{
	Super::BeginPlay();	
}

void AServer::Listen(TSharedRef<FInternetAddr> Address)
{
	Listener = new FTcpListener(FIPv4Endpoint(Address), FTimespan(0, 0, 1));
	Listener->OnConnectionAccepted().BindUObject(this, &AServer::OnConnection);
	Listener->Init();
}

bool AServer::OnConnection(class FSocket* InClientSocket, const FIPv4Endpoint& ClientEndpoint)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("~~~~~~ Connection happened!")));
	ClientSocket = InClientSocket;

	return true;
}

void AServer::Connect(TSharedRef<FInternetAddr> Address)
{
	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	ServerSocket = SocketSubsystem->CreateSocket(NAME_Stream, FString(TEXT("Foo")), false);

	if (ServerSocket)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting!")));
		
		if (ServerSocket->Connect(*Address))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connected")));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("NOT connected :(")));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No connect socket created")));
	}
}

//
//void AServer::Laaaaaauuuunch()
//{
//	//IP = 127.0.0.1, Port = 8890 for my Python test case
//	if (!StartTCPReceiver("RamaSocketListener", "127.0.0.1", 8890))
//	{
//		//UE_LOG  "TCP Socket Listener Created!"
//		return;
//	}
//
//	//UE_LOG  "TCP Socket Listener Created! Yay!"
//}



//
//
//bool AServer::StartTCPReceiver(
//	const FString& YourChosenSocketName,
//	//const FString& TheIP,
//	const int32 ThePort
//) {
//	bool canBind = false;
//	TSharedRef<FInternetAddr> LocalAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);
//	
//	if (LocalAddr->IsValid())
//	{
//		FString Addr = LocalAddr->ToString(true);
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Starting listener on address %s"), *Addr));
//		//GLog->Log(LocalAddr->ToString(false)); // if you want to append the port (true) or not (false).
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No valid IP")));
//	}
///*
//	ListenerSocket = CreateTCPConnectionListener(YourChosenSocketName, TheIP, ThePort);
//
//	if (!ListenerSocket)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("StartTCPReceiver>> Listen socket could not be created! ~> %s %d"), *TheIP, ThePort));
//		return false;
//	}
//
//	GetWorldTimerManager().SetTimer(NetTimer, this, &AServer::TCPConnectionListener, 0.01, true);*/
//
//	return true;
//}
//
//
////bool AServer::StartTCPReceiver(
////	const FString& YourChosenSocketName,
////	const FString& TheIP,
////	const int32 ThePort
////) {
////	ListenerSocket = CreateTCPConnectionListener(YourChosenSocketName, TheIP, ThePort);
////
////	if (!ListenerSocket)
////	{
////		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("StartTCPReceiver>> Listen socket could not be created! ~> %s %d"), *TheIP, ThePort));
////		return false;
////	}
////
////	GetWorldTimerManager().SetTimer(NetTimer, this, &AServer::TCPConnectionListener, 0.01, true);
////
////	return true;
////}
//
//FSocket* AServer::CreateTCPConnectionListener(
//	const FString& YourChosenSocketName,
//	const FString& TheIP,
//	const int32 ThePort,
//	const int32 ReceiveBufferSize
//)
//{
//	uint8 IP4Nums[4];
//	if (!FormatIP4ToNumber(TheIP, IP4Nums))
//	{
//		//VShow("Invalid IP! Expecting 4 parts separated by .");
//		return false;
//	}
//
//	//Create Socket
//	FIPv4Endpoint Endpoint(FIPv4Address(IP4Nums[0], IP4Nums[1], IP4Nums[2], IP4Nums[3]), ThePort);
//	FSocket* ListenSocket = FTcpSocketBuilder(*YourChosenSocketName)
//		.AsReusable()
//		.BoundToEndpoint(Endpoint)
//		.Listening(8);
//
//	//Set Buffer Size
//	int32 NewSize = 0;
//	ListenSocket->SetReceiveBufferSize(ReceiveBufferSize, NewSize);
//
//	//Done!
//	return ListenSocket;
//}
//
////Rama's TCP Connection Listener
//void AServer::TCPConnectionListener()
//{
//	//~~~~~~~~~~~~~
//	if (!ListenerSocket) return;
//	//~~~~~~~~~~~~~
//
//	//Remote address
//	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
//	bool Pending;
//
//	// handle incoming connections
//	if (ListenerSocket->HasPendingConnection(Pending) && Pending)
//	{
//		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//		//Already have a Connection? destroy previous
//		if (ConnectionSocket)
//		{
//			ConnectionSocket->Close();
//			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectionSocket);
//		}
//		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//		//New Connection receive!
//		ConnectionSocket = ListenerSocket->Accept(*RemoteAddress, TEXT("RamaTCP Received Socket Connection"));
//
//		if (ConnectionSocket != NULL)
//		{
//			//Global cache of current Remote Address
//			RemoteAddressForConnection = FIPv4Endpoint(RemoteAddress);
//
//			//UE_LOG "Accepted Connection! WOOOHOOOO!!!";
//
//			//can thread this too
//			GetWorldTimerManager().SetTimer(NetTimer, this, &AServer::TCPSocketListener, 0.01, true);
//		}
//	}
//}
//
////Rama's TCP Socket Listener
//void AServer::TCPSocketListener()
//{
//	if (!ConnectionSocket)
//	{
//		return;
//	}
//
//	//Binary Array!
//	TArray<uint8> ReceivedData;
//
//	uint32 Size;
//	while (ConnectionSocket->HasPendingData(Size))
//	{
//		ReceivedData.Init(FMath::Min(Size, 65507u), 0);
//
//		int32 Read = 0;
//		ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);
//
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Data Read! %d"), ReceivedData.Num()));
//	}
//
//
//	if (ReceivedData.Num() <= 0)
//	{
//		return;
//	}
//
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Data Bytes Read ~> %d"), ReceivedData.Num()));
//
//	const FString ReceivedUE4String = StringFromBinaryArray(ReceivedData);
//
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("As String Data ~> %s"), *ReceivedUE4String));
//}
//
////Format IP String as Number Parts
//bool AServer::FormatIP4ToNumber(const FString& TheIP, uint8(&Out)[4])
//{
//	//IP Formatting
//	TheIP.Replace(TEXT(" "), TEXT(""));
//
//	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	//						   IP 4 Parts
//
//	//String Parts
//	TArray<FString> Parts;
//	TheIP.ParseIntoArray(Parts, TEXT("."), true);
//	if (Parts.Num() != 4)
//		return false;
//
//	//String to Number Parts
//	for (int32 i = 0; i < 4; ++i)
//	{
//		Out[i] = FCString::Atoi(*Parts[i]);
//	}
//
//	return true;
//}
//
//
//
////Rama's String From Binary Array
////This function requires 
////		#include <string>
//FString AServer::StringFromBinaryArray(const TArray<uint8>& BinaryArray)
//{
//	//Create a string from a byte array!
//	std::string cstr(reinterpret_cast<const char*>(BinaryArray.GetData()), BinaryArray.Num());
//	return FString(cstr.c_str());
//}
