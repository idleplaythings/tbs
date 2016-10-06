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
		TCPServer = NULL;
	}

	FMemory::Free(LevelDataBuffer);
	LevelDataBuffer = nullptr;
}

void ATBSGameState::StartGameplay()
{
	InitGrid(FIntVector(5000, 5000, 18));
	InitGridUI();

	if (HasAuthority())
	{
		InitFactoriesAndManagers();
		Grid->OnActorNoLongerVisible.AddDynamic(this, &ATBSGameState::ForceCloseActorChannel);
	}
}

void ATBSGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (TCPServer)
		{
			if (!TCPServer->NetworkMessageQueue.IsEmpty())
			{
				FNetworkMessage Message;
				if (TCPServer->NetworkMessageQueue.Dequeue(Message))
				{
					RespondToClientMessage(Message);
					FMemory::Free(Message.Data);
				}
			}
		}
	}	
}

void ATBSGameState::RespondToClientMessage(FNetworkMessage Message)
{
	//uint8_t* Temp = new uint8_t[Message.Length + 1];
	//memcpy(Temp, Message.Data, Message.Length);
	//Temp[Message.Length] = '\0';

	if (LevelDataBufferLength > 0)
	{
		int32 BytesSent = 0;
		TCPServer->Send(Message.ConnectionId, (uint8_t)0x01, LevelDataBuffer, LevelDataBufferLength, BytesSent);
	}
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

	CreateRandomLevel();

	bool CanBind = false;
	TSharedRef<FInternetAddr> Address = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, CanBind);
	Address->SetPort(10021);

	if (Address->IsValid())
	{
		TCPServer = new TBSTCPServer();
		TCPServer->Listen(Address);

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Side channel listening on %s"), *Address->ToString(true)));

		for (auto& It : PlayerControllers)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Open side channel")));
			(*It.Value).Client_OpenSideChannelConnection(Address->ToString(true));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("No ip found!")));
	}

	Grid->ReindexUnits();

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Side channel done")));
}

void ATBSGameState::CreateRandomLevel()
{
	//int32 Props = 1000000;
	//int32 Props = 500000;
	//uint32 Props = 41600;
	uint32 Props = 10000;
	//uint32 Props = 1;

	//LevelDataBuffer = (uint8_t*)FMemory::Malloc(sizeof(FMapMeta) + Props * sizeof(FProp));
	LevelDataBuffer = (uint8_t*)FMemory::Malloc(Props * sizeof(FProp));

	// Generate random meta data here...

	while (Grid->PropCount() < Props)
	{
		FIntVector Coordinates = FIntVector(
			FMath::RandRange(0, (Grid->GridDimensions.X - 1) / 10) * 10,
			FMath::RandRange(0, (Grid->GridDimensions.Y - 1) / 10) * 10,
			0
		);

		if (!Grid->IsAccessible(Coordinates))
		{
			continue;
		}

		//for (int i = 0; i < 6; i++)
		//{
			//Coordinates.Z = i;

			FProp Prop;
			Prop.Coordinates = Coordinates;
			Prop.Rotation = FMath::RandRange(0, 3) * 90;
			Prop.Dimensions = FIntVector(1, 3, FMath::RandRange(1,6));
			Prop.BlocksAccess = true;

			Grid->AddProp(Prop);

			FMemory::Memcpy(LevelDataBuffer + LevelDataBufferLength, &Prop, sizeof(FProp));
			LevelDataBufferLength += sizeof(FProp);
		//}
	}
}

void ATBSGameState::SpawnNewProps(FIntVector Coordinates)
{
	if (!Grid->IsAccessible(Coordinates))
	{
		return;
	}

	uint8_t* PropsData = (uint8_t*)FMemory::Malloc(sizeof(FProp) * 6);
	uint32 PropsDataLength = 0;

	for (int i = 0; i < 6; i++)
	{
		Coordinates.Z = i;

		FProp Prop;
		Prop.Coordinates = Coordinates;
		Prop.Rotation = (float)FMath::RandRange(0, 3) * 90;
		Prop.BlocksAccess = true;

		Grid->AddProp(Prop);

		memcpy(PropsData + PropsDataLength, &Prop, sizeof(FProp));
		PropsDataLength += sizeof(FProp);		
	}

	TCPServer->SendAll((uint8_t) 0x01, PropsData, PropsDataLength);
	Grid->ReindexUnits();
	FMemory::Free(PropsData);
}

void ATBSGameState::Bomb(FIntVector Coordinates)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Bombing")));

	uint8_t* BombData = (uint8_t*)FMemory::Malloc(sizeof(FIntVector) * 6);
	uint32 BombDataLength = 0;

	for (int32 Z = 0; Z < 6; Z++)
	{
		FIntVector TempCoordinates = FIntVector(Coordinates.X, Coordinates.Y, Z);

		Grid->RemovePropsAt(TempCoordinates);

		memcpy(BombData + BombDataLength, &TempCoordinates, sizeof(FIntVector));
		BombDataLength += sizeof(FIntVector);
	}

	TCPServer->SendAll((uint8_t)0x02, BombData, BombDataLength);	

	AActor* Explosion = GetWorld()->SpawnActor<AActor>(ExplosionClass);
	Explosion->SetActorLocation(GridUI->GetCoordinateLocations(Coordinates).Center);

	Grid->ReindexUnits();

	FMemory::Free(BombData);
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

	while (UnitsSpawned < 10)
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
