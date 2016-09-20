// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSUnit.h"
#include "TBSGrid.h"
#include "TBSGridUI.h"
#include "UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "TBSGameState.h"

ATBSGameState::ATBSGameState()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UBlueprint> ExplosionBlueprint(TEXT("Blueprint'/Game/Blueprints/Explosion.Explosion'"));
	ExplosionClass = (UClass*)ExplosionBlueprint.Object->GeneratedClass;	
}

ATBSGameState::~ATBSGameState()
{
	if (TCPServer)
	{
		TCPServer->Stop();
		delete TCPServer;
	}

	FMemory::Free(PropsData);
	PropsData = nullptr;

	//delete PropsBuffer;
	//PropsBuffer = nullptr;
}

void ATBSGameState::StartGameplay()
{
	InitGrid(FIntVector(3000, 3000, 18));
	InitGridUI();

	if (HasAuthority())
	{
		InitFactoriesAndManagers();

		//int PropsSpawned = 0;

		//while (PropsSpawned < 20)
		//{
		//	FIntVector Coordinates = FIntVector(FMath::RandRange(10, 90)*10, FMath::RandRange(10, 90)*10, 0);
		//	int32 Rotation = (float) FMath::RandRange(0, 3) * 90;

		//	if (Grid->SelectProp(Coordinates))
		//	{
		//		continue;
		//	}

		//	//Grid->AddProp(PropFactory->CreateWall(Coordinates, FRotator(0.0, Rotation, 0.0)));
		//	Grid->AddProp(PropFactory->CreateBlock(Coordinates, FIntVector(1, 1, 6), FRotator(0.0, Rotation, 0.0)));

		//	PropsSpawned += 1;
		//}
		//PropManager->ResetProps();

		//ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
		//bool CanBind = false;
		//TSharedRef<FInternetAddr> Address = SocketSubsystem->GetLocalHostAddr(*GLog, CanBind);
		//Address->SetPort(10003);

		//FString AddressStr = Address->ToString(true);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Starting listener on address %s"), *AddressStr));

		Grid->OnActorNoLongerVisible.AddDynamic(this, &ATBSGameState::ForceCloseActorChannel);
	}
}

void ATBSGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Tick")));

		if (TCPServer)
		{
			if (!TCPServer->NetworkMessageQueue.IsEmpty())
			{
				NetworkMessage Message;
				if (TCPServer->NetworkMessageQueue.Dequeue(Message))
				{
					char* Temp = new char[Message.Length + 1];
					memcpy(Temp, Message.Data, Message.Length);
					Temp[Message.Length] = '\0';

					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Server received: %s"), ANSI_TO_TCHAR(Temp)));

					//std::string Response = "Server says ohai!";
					//TCPServer->Send(Message.ConnectionId, Response.c_str(), Response.length());
					TCPServer->Send(Message.ConnectionId, PropsData, PropsDataLength);
				}
			}
		}
	}	
}


void ATBSGameState::OnMessage(FString Message)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(ATBSGameState::OnTCPMessage: %s"), *Message));
}

void ATBSGameState::AddPlayer(APlayerController* PlayerController)
{
	int32 PlayerNumber = NumberOfPlayers;

	InitPlayerController(PlayerNumber, PlayerController);
	SpawnUnits(PlayerNumber);

	NumberOfPlayers++;
}

/**
 * Force close a replication channel associated with an actor.

 * By default UE4 will keep the replication channel open for an actor for five seconds after the actor is
 * no longer relevant, which is something we specifically don't want as that might reveal hidden
 * information about unit that has moved out of enemy LOF.
 */
void ATBSGameState::ForceCloseActorChannel(int32 PlayerNumber, AActor* Actor)
{
	if (HasAuthority())
	{
		UNetConnection* Connection = PlayerControllers[PlayerNumber]->GetNetConnection();
		UActorChannel* Channel = Connection->ActorChannels.FindRef(Actor);

		if (Channel)
		{
			Channel->Close();
		}		
	}
}

void ATBSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATBSGameState, NumberOfPlayers);
}

void ATBSGameState::InitGrid(FIntVector Dimensions)
{
	Grid = GetWorld()->SpawnActor<ATBSGrid>(ATBSGrid::StaticClass());
	Grid->InitialiseGrid(Dimensions);
}

void ATBSGameState::InitGridUI()
{
	if (Grid)
	{
		GridUI = GetWorld()->SpawnActor<ATBSGridUI>(ATBSGridUI::StaticClass());
		GridUI->RenderGrid(Grid);
	}
}

void ATBSGameState::InitFactoriesAndManagers()
{
	PropFactory = GetWorld()->SpawnActor<ATBSPropFactory>(ATBSPropFactory::StaticClass());
	UnitFactory = GetWorld()->SpawnActor<ATBSUnitFactory>(ATBSUnitFactory::StaticClass());

	if (Grid && GridUI)
	{
		PropManager = GetWorld()->SpawnActor<ATBSPropManager>(ATBSPropManager::StaticClass());
		PropManager->Initialise(Grid, GridUI);

		UnitManager = GetWorld()->SpawnActor<ATBSUnitManager>(ATBSUnitManager::StaticClass());
		UnitManager->Initialise(Grid, GridUI);
	}
}

void ATBSGameState::InitPlayerController(int32 PlayerNumber, APlayerController* PlayerController)
{
	ATBSPlayerController* Controller = Cast<ATBSPlayerController>(PlayerController);
	Controller->PlayerNumber = PlayerNumber;
	Controller->OnClientReady.AddDynamic(this, &ATBSGameState::ClientReady);

	PlayerControllers.Add(PlayerNumber, Controller);	
}

void ATBSGameState::ClientReady()
{
	NumberOfPlayersReady += 1;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Clients ready %i/%i"), NumberOfPlayersReady, NumberOfPlayersExpected));

	if (NumberOfPlayersReady == NumberOfPlayersExpected)
	{
		AllClientsReady();
	}
}

void ATBSGameState::AllClientsReady()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("All clients ready")));

	TCPServer = new TBSTCPServer();
	TCPServer->Listen(FString("192.168.0.107"), 10011);

	PropsData = (char*)FMemory::Malloc((sizeof(FProp) + 1) * PropsToGenerate);

	while (Grid->PropCount() < PropsToGenerate / 6)
	{
		FIntVector Coordinates = FIntVector(
			FMath::RandRange(0, Grid->GridDimensions.X / 10) * 10,
			FMath::RandRange(0, Grid->GridDimensions.Y / 10) * 10,
			0
		);

		if (!Grid->IsAccessible(Coordinates))
		{
			continue;
		}

		for (int i = 0; i < 6; i++)
		{
			Coordinates.Z = i;

			FProp Prop;
			Prop.Coordinates = Coordinates;
			Prop.Rotation = (float)FMath::RandRange(0, 3) * 90;
			Prop.BlocksAccess = true;

			char* PropsBuffer = reinterpret_cast<char*>(&Prop);
			memset(PropsData + PropsDataLength, 0x01, 1);
			memcpy(PropsData + 1 + PropsDataLength, PropsBuffer, sizeof(FProp));
			PropsDataLength += sizeof(FProp) + 1;

			//delete PropsBuffer;
			//PropsBuffer = nullptr;

			Grid->AddProp(Prop);
		}
	}

	for (auto& It : PlayerControllers)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Open side channel")));
		(*It.Value).Client_OpenSideChannelConnection();
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Side channel done")));
}

void ATBSGameState::SpawnNewProps(FIntVector Coordinates)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Spawning new props")));

	//FIntVector Coordinates = FindFreeCoordinates();

	//if (Coordinates == FIntVector::NoneValue)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Unable to find free coordinates!")));
	//	return;
	//}

	if (!Grid->IsAccessible(Coordinates))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Coordinates (%i, %i, %i) blocked!"), Coordinates.X, Coordinates.Y, Coordinates.Z));
		return;
	}

	char* PropsData = (char*)FMemory::Malloc((sizeof(FProp) + 1) * 6);
	uint32 PropsDataLength = 0;

	for (int i = 0; i < 6; i++)
	{
		Coordinates.Z = i;

		FProp Prop;
		Prop.Coordinates = Coordinates;
		Prop.Rotation = (float)FMath::RandRange(0, 3) * 90;
		Prop.BlocksAccess = true;

		char* PropsBuffer = reinterpret_cast<char*>(&Prop);
		memset(PropsData + PropsDataLength, 0x01, 1);
		memcpy(PropsData + 1 + PropsDataLength, PropsBuffer, sizeof(FProp));
		PropsDataLength += sizeof(FProp) + 1;

		//FMemory::Free(PropsBuffer);

		Grid->AddProp(Prop);
	}

	TCPServer->SendAll(PropsData, PropsDataLength);
}

void ATBSGameState::Bomb(FIntVector Coordinates)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Bombing")));

	char* BombData = (char*)FMemory::Malloc((sizeof(FIntVector) + 1) * 11 * 11 * 6);
	uint32 BombDataLength = 0;

	for (int32 X = Coordinates.X - 50; X <= Coordinates.X + 50; X = X + 10)
	{
		for (int32 Y = Coordinates.Y - 50; Y <= Coordinates.Y + 50; Y = Y + 10)
		{
			for (int32 Z = 0; Z < 6; Z++)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Bombing (%i, %i, %i)"), X, Y, Z));

				//Coordinates.X = X;
				//Coordinates.Y = Y;
				//Coordinates.Z = Z;

				FIntVector TempCoordinates = FIntVector(X, Y, Z);

				char* CoordinateBuffer = reinterpret_cast<char*>(&TempCoordinates);
				memset(BombData + BombDataLength, 0x02, 1);
				memcpy(BombData + 1 + BombDataLength, CoordinateBuffer, sizeof(FIntVector));
				BombDataLength += sizeof(FIntVector) + 1;

				//FMemory::Free(CoordinateBuffer);

				Grid->RemovePropsAt(TempCoordinates);
			}
		}
	}

	TCPServer->SendAll(BombData, BombDataLength);

	AActor* Explosion = GetWorld()->SpawnActor<AActor>(ExplosionClass);
	Explosion->SetActorLocation(GridUI->GetCoordinateLocations(Coordinates).Center);
	

	//FIntVector Coordinates = FindFreeCoordinates();

	//if (Coordinates == FIntVector::NoneValue)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Unable to find free coordinates!")));
	//	return;
	//}

	//if (!Grid->IsAccessible(Coordinates))
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Coordinates (%i, %i, %i) blocked!"), Coordinates.X, Coordinates.Y, Coordinates.Z));
	//	return;
	//}

	//char* PropsData = (char*)FMemory::Malloc(sizeof(FProp) * 6);
	//uint32 PropsDataLength = 0;

	//for (int i = 0; i < 6; i++)
	//{
	//	Coordinates.Z = i;

	//	FProp Prop;
	//	Prop.Coordinates = Coordinates;
	//	Prop.Rotation = (float)FMath::RandRange(0, 3) * 90;
	//	Prop.BlocksAccess = true;

	//	char* PropsBuffer = reinterpret_cast<char*>(&Prop);
	//	memcpy(PropsData + PropsDataLength, PropsBuffer, sizeof(FProp));
	//	PropsDataLength += sizeof(FProp);

	//	//delete PropsBuffer;
	//	//PropsBuffer = nullptr;

	//	Grid->AddProp(Prop);
	//}

	//TCPServer->SendAll(PropsData, PropsDataLength);
}

FIntVector ATBSGameState::FindFreeCoordinates()
{
	bool Found = false;
	uint32 Tries = 2000;

	while (!Found && Tries > 0)
	{
		FIntVector Coordinates = FIntVector(
			FMath::RandRange(0, Grid->GridDimensions.X / 10) * 10,
			FMath::RandRange(0, Grid->GridDimensions.Y / 10) * 10,
			0
		);

		if (!Grid->IsAccessible(Coordinates))
		{
			return Coordinates;
		}

		Tries--;
	}

	return FIntVector::NoneValue;
}

void ATBSGameState::SpawnUnits(int32 PlayerNumber)
{
	int32 UnitsSpawned = 0;
	int32 XOffset = 0;

	int32 Area = 10;
	int32 HalfX = Grid->GridDimensions.X / 10 / 2;
	int32 HalfY = Grid->GridDimensions.Y / 10 / 2;

	if (PlayerNumber == 0)
	{
		XOffset = -10;
	}

	while (UnitsSpawned < 3)
	{
		FIntVector Coordinates;

		if (UnitsSpawned == 0)
		{
			Coordinates = FIntVector(
				FMath::RandRange(HalfX - Area + XOffset, HalfX + Area + XOffset) * 10 + 5,
				FMath::RandRange(HalfY - Area, HalfX + Area) * 10 + 5,
				0
			);
		}
		else
		{
			Coordinates = FIntVector(
				FMath::RandRange(HalfX - Area + XOffset, HalfX + Area + XOffset) * 10,
				FMath::RandRange(HalfY - Area, HalfX + Area) * 10,
				0
			);
		}

		if (Grid->SelectUnit(Coordinates))
		{
			continue;
		}

		if (Grid->SelectProp(Coordinates))
		{
			continue;
		}

		ATBSUnit* Unit;

		if (UnitsSpawned == 0)
		{
			Unit = UnitFactory->CreateUnit(Coordinates, FRotator(0.0, 0.0, 0.0));			
		}
		else if (UnitsSpawned == 1)
		{
			Unit = UnitFactory->CreateLargeUnit(Coordinates, FRotator(0.0, 0.0, 0.0));
		}
		else
		{
			Unit = UnitFactory->CreateSmallUnit(Coordinates, FRotator(0.0, 0.0, 0.0));
		}
		
		Unit->PlayerNumber = PlayerNumber;

		Grid->AddUnit(Unit);
		UnitManager->ResetUnit(Unit);

		UnitsSpawned += 1;
	}

	Grid->ReindexUnits();
}
