// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include <string>
#include "Networking.h"
#include "Server.generated.h"

UCLASS()
class TBS_API AServer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AServer();
	~AServer();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	//virtual void Tick( float DeltaSeconds ) override;

	void Listen(TSharedRef<FInternetAddr> Address);
	void Connect(TSharedRef<FInternetAddr> Address);

	//FSocket* ListenerSocket;
	//FSocket* ConnectionSocket;
	//FIPv4Endpoint RemoteAddressForConnection;

	//bool StartTCPReceiver(
	//	const FString& YourChosenSocketName,
	//	const int32 ThePort
	//);

	////bool StartTCPReceiver(
	////	const FString& YourChosenSocketName,
	////	const FString& TheIP,
	////	const int32 ThePort
	////);

	//FSocket* CreateTCPConnectionListener(
	//	const FString& YourChosenSocketName,
	//	const FString& TheIP,
	//	const int32 ThePort,
	//	const int32 ReceiveBufferSize = 2 * 1024 * 1024
	//);

	////Timer functions, could be threads
	//void TCPConnectionListener(); 	//can thread this eventually
	//void TCPSocketListener();		//can thread this eventually


	//								//Format String IP4 to number array
	//bool FormatIP4ToNumber(const FString& TheIP, uint8(&Out)[4]);

	////Rama's StringFromBinaryArray
	//FString StringFromBinaryArray(const TArray<uint8>& BinaryArray);	

	bool OnConnection(class FSocket* ClientSocket, const FIPv4Endpoint& ClientEndpoint);
	FSocket* ClientSocket;
	FSocket* ServerSocket;

	FTcpListener* Listener;
	FTimerHandle NetTimer;
};
