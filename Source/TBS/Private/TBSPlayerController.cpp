// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "UnrealNetwork.h"
#include "TBSPlayerState.h"
#include "TBSGameMode.h"
#include "TBSGameState.h"
#include "TBSUIDefaultContext.h"
#include "TBSUIContextAxisEvent.h"
#include "TBSUIContextCoordinateEvent.h"
#include <string>
#include "TBSPlayerController.h"

ATBSPlayerController::ATBSPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	EnableMouse();
}

ATBSPlayerController::~ATBSPlayerController()
{
	if (TCPClient)
	{
		TCPClient->Stop();
	}
}


void ATBSPlayerController::EnableMouse()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
	bEnableTouchOverEvents = true;
}

void ATBSPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATBSPlayerController, PlayerNumber);
}

void ATBSPlayerController::BeginPlay()
{
	if (IsLocalPlayerController())
	{
		ClassLoader = GetWorld()->SpawnActor<ATBSClassLoader>(ATBSClassLoader::StaticClass());
		ClassLoader->Initialise(this);
		ClassLoader->OnClassesLoaded.AddDynamic(this, &ATBSPlayerController::OnClassesLoaded);
	}
}

void ATBSPlayerController::OnClassesLoaded()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ATBSPlayerController::ClassesLoaded")));

	ClassesLoaded = true;

	UIContextStack = new TBSUIContextStack(ClassLoader);
	UIContextStack->PushContext(new TBSUIDefaultContext);

	InputComponent->BindAction("ActionLevelUp", IE_Pressed, this, &ATBSPlayerController::ViewLevelUp);
	InputComponent->BindAction("ActionLevelDown", IE_Pressed, this, &ATBSPlayerController::ViewLevelDown);
	InputComponent->BindAction("ActionRotateCameraRight", IE_Pressed, this, &ATBSPlayerController::RotateCameraRight);
	InputComponent->BindAction("ActionRotateCameraLeft", IE_Pressed, this, &ATBSPlayerController::RotateCameraLeft);
	InputComponent->BindAction("ActionZoomIn", IE_Pressed, this, &ATBSPlayerController::ZoomCameraIn);
	InputComponent->BindAction("ActionZoomOut", IE_Pressed, this, &ATBSPlayerController::ZoomCameraOut);
	InputComponent->BindAction("TogglePerspectiveCamera", IE_Pressed, this, &ATBSPlayerController::TogglePerspectiveCamera);
	InputComponent->BindAction("ActionMouseLeft", IE_Pressed, this, &ATBSPlayerController::MouseLeft);
	InputComponent->BindAction("ActionMouseRight", IE_Pressed, this, &ATBSPlayerController::MouseRight);
	InputComponent->BindAction("ActionEscape", IE_Pressed, this, &ATBSPlayerController::Escape);
	InputComponent->BindAction("ActionPropDebug", IE_Pressed, this, &ATBSPlayerController::PropDebug);
	InputComponent->BindAxis("AxisMoveCameraForward", this, &ATBSPlayerController::MoveCameraForward);
	InputComponent->BindAxis("AxisMoveCameraRight", this, &ATBSPlayerController::MoveCameraRight);

	// Testing stuff
	InputComponent->BindAction("ActionDebugMessage", IE_Pressed, this, &ATBSPlayerController::SendDebugMessage);
	InputComponent->BindAction("ActionNewProp", IE_Pressed, this, &ATBSPlayerController::NewProp);
	InputComponent->BindAction("ActionBomb", IE_Pressed, this, &ATBSPlayerController::Bomb);

	Server_ClientReady();
	TraceGrid();

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Controller ready, server address %s"), *GetServerNetworkAddress()));

	//ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	//TSharedRef<FInternetAddr> Address = SocketSubsystem->CreateInternetAddr();
}

void ATBSPlayerController::Client_OpenSideChannelConnection_Implementation(const FString &Address)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Init side channel connection, host %s"), *Address));

	SideChannelAddress = Address;

	TCPClient = new TBSTCPClient();
	GetWorldTimerManager().SetTimer(
		SideChannelConnectionTimer,
		this,
		&ATBSPlayerController::TrySideChannelConnection,
		SideChannelConnectionTimeout,
		true
	);
}

void ATBSPlayerController::TrySideChannelConnection()
{
	FString Host;
	FString Port;

	SideChannelAddress.Split(FString(":"), &Host, &Port);

	bool IsValidAddr = true;
	TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Addr->SetIp(*Host, IsValidAddr);
	Addr->SetPort(FCString::Atoi(*Port));
	

	if (TCPClient->Connect(Addr))
	{
		GetWorldTimerManager().ClearTimer(SideChannelConnectionTimer);
		std::string Message = "Client says Hello!";
		int32 BytesSent = 0;
		TCPClient->Send((uint8_t)0x10, (uint8_t*)Message.c_str(), Message.length(), BytesSent);
	}

	if (SideChannelConnectionAttempts-- < 1)
	{
		GetWorldTimerManager().ClearTimer(SideChannelConnectionTimer);
	}
}

void ATBSPlayerController::TraceGrid()
{
	if (ClassesLoaded)
	{
		TraceChannel = ECollisionChannel::ECC_GameTraceChannel1;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Tracing grid %i"), (int32)TraceChannel));
	}
	
}

void ATBSPlayerController::TraceProps()
{
	if (ClassesLoaded)
	{		
		TraceChannel = ECollisionChannel::ECC_GameTraceChannel2;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Tracing props %i"), (int32)TraceChannel));
	}
}

void ATBSPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (ClassesLoaded)
	{
		FHitResult Result;
		GetHitResultUnderCursor(TraceChannel, true, Result);

		if (TraceChannel == ECollisionChannel::ECC_GameTraceChannel1)
		{
			ClassLoader->GridUI->HandleGridHitResult(Result);
		}
		else
		{
			ClassLoader->GridUI->HandlePropHitResult(Result, ClassLoader->DefaultPawn->GetCameraViewAngle());
		}		

		if (ClassLoader->GridUI->CoordinatesChanged)
		{
			TBSUIContextCoordinateEvent Event = TBSUIContextCoordinateEvent(FName(TEXT("TileHoverEnd")), ClassLoader->GridUI->CurrentCoordinates);
			UIContextStack->HandleEvent(Event);

			if (ClassLoader->GridUI->CursorOnGrid)
			{
				TBSUIContextCoordinateEvent Event = TBSUIContextCoordinateEvent(FName(TEXT("TileHoverBegin")), ClassLoader->GridUI->CurrentCoordinates);
				UIContextStack->HandleEvent(Event);
			}
		}

		if (TCPClient)
		{
			if (!TCPClient->NetworkMessageQueue.IsEmpty())
			{
				int32 MessageNum = 0;
				FNetworkMessage Message;
				while (TCPClient->NetworkMessageQueue.Dequeue(Message))
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Message of length %i"), Message.Length));
					MessageNum++;
					uint8_t* MessagePtr = Message.Data;

					if (*(uint8_t*)MessagePtr == 0x01)
					{
						MessagePtr += 1;

						while (MessagePtr < Message.Data + Message.Length)
						{
							FProp Prop;
							memcpy(&Prop, MessagePtr, sizeof(FProp));

							//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("received prop id %i"), Prop.Id));

							ClassLoader->Grid->AddProp(Prop);

							FIntVector BlockCell = FIntVector(
								FMath::FloorToInt(Prop.Coordinates.X / 300),
								FMath::FloorToInt(Prop.Coordinates.Y / 300),
								0
							);

							ATBSProp_Block** BlockPtr = BlockMap.Find(BlockCell);

							if (!BlockPtr)
							{
								Block = GetWorld()->SpawnActor<ATBSProp_Block>(ATBSProp_Block::StaticClass());
								Block->SetActorLocation(FVector(0.0, 0.0, 0.0));
								BlockMap.Add(BlockCell, Block);
							}
							else
							{
								Block = *BlockPtr;
							}

							FCoordinateLocations Locations = ClassLoader->GridUI->GetCoordinateLocations(Prop.Coordinates);
							FTransform InstanceTransform(
								FRotator(0.0, (float)Prop.Rotation, 0.0),
								Locations.Center,
								FVector(((float)1 / 2) * Prop.Dimensions.X, ((float)1 / 2) * Prop.Dimensions.Y, ((float)1 / 2) * Prop.Dimensions.Z)
							);

							Block->SpawnInstance(Prop.Id, InstanceTransform);

							MessagePtr += sizeof(FProp);
						}
					}
					else if (*(uint8_t*)MessagePtr == 0x02)
					{
						MessagePtr += 1;

						while (MessagePtr < Message.Data + Message.Length)
						{
							FIntVector Coordinates;
							memcpy(&Coordinates, MessagePtr, sizeof(FIntVector));

							//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Received remove props at (%i, %i, %i)"), Coordinates.X, Coordinates.Y, Coordinates.Z));

							//ClassLoader->Grid->RemovePropsAt(Coordinates);

							for (auto& Prop : ClassLoader->Grid->GetPropsAt(Coordinates))
							{
								//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("found a prop %i"), Prop.Id));
								
								FIntVector BlockCell = FIntVector(
									FMath::FloorToInt(Prop.Coordinates.X / 300),
									FMath::FloorToInt(Prop.Coordinates.Y / 300),
									0
								);

								ATBSProp_Block** BlockPtr = BlockMap.Find(BlockCell);

								if (BlockPtr)
								{

									//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("got block")));

									(*BlockPtr)->RemoveInstance(Prop.Id);

									ClassLoader->Grid->RemovePropById(Prop.Id);
								}

							}

							MessagePtr += sizeof(FIntVector);
						}
					}

					FMemory::Free(Message.Data);
				}

				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("%i Messages handled"), MessageNum));
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("%i Props"), ClassLoader->Grid->PropCount()));
			}
		}
	}
}

void ATBSPlayerController::ViewLevelUp()
{
	TBSUIContextEvent Event = TBSUIContextEvent(FName(TEXT("ViewLevelUp")));
	UIContextStack->HandleEvent(Event);
}

void ATBSPlayerController::ViewLevelDown()
{
	TBSUIContextEvent Event = TBSUIContextEvent(FName(TEXT("ViewLevelDown")));
	UIContextStack->HandleEvent(Event);
}

void ATBSPlayerController::RotateCameraRight()
{
	TBSUIContextEvent Event = TBSUIContextEvent(FName(TEXT("RotateCameraRight")));
	UIContextStack->HandleEvent(Event);
}

void ATBSPlayerController::RotateCameraLeft()
{
	TBSUIContextEvent Event = TBSUIContextEvent(FName(TEXT("RotateCameraLeft")));
	UIContextStack->HandleEvent(Event);
}

void ATBSPlayerController::ZoomCameraIn()
{
	TBSUIContextEvent Event = TBSUIContextEvent(FName(TEXT("ZoomCameraIn")));
	UIContextStack->HandleEvent(Event);
}

void ATBSPlayerController::ZoomCameraOut()
{
	TBSUIContextEvent Event = TBSUIContextEvent(FName(TEXT("ZoomCameraOut")));
	UIContextStack->HandleEvent(Event);
}

void ATBSPlayerController::TogglePerspectiveCamera()
{
	TBSUIContextEvent Event = TBSUIContextEvent(FName(TEXT("TogglePerspectiveCamera")));
	UIContextStack->HandleEvent(Event);
}

void ATBSPlayerController::MoveCameraForward(float AxisValue)
{
	TBSUIContextAxisEvent Event = TBSUIContextAxisEvent(FName(TEXT("MoveCameraForward")), AxisValue);
	UIContextStack->HandleEvent(Event);
}

void ATBSPlayerController::MoveCameraRight(float AxisValue)
{
	TBSUIContextAxisEvent Event = TBSUIContextAxisEvent(FName(TEXT("MoveCameraRight")), AxisValue);
	UIContextStack->HandleEvent(Event);
}

void ATBSPlayerController::MoveCameraForwardOffset(float AxisValue)
{
	// This event is not triggered via the input component but by the HUD, so
	// ensure we have all the classes loaded before attempting to handle it
	if (ClassesLoaded)
	{
		TBSUIContextAxisEvent Event = TBSUIContextAxisEvent(FName(TEXT("MoveCameraForwardOffset")), AxisValue);
		UIContextStack->HandleEvent(Event);
	}
}

void ATBSPlayerController::MoveCameraRightOffset(float AxisValue)
{
	// This event is not triggered via the input component but by the HUD, so
	// ensure we have all the classes loaded before attempting to handle it
	if (ClassesLoaded)
	{
		TBSUIContextAxisEvent Event = TBSUIContextAxisEvent(FName(TEXT("MoveCameraRightOffset")), AxisValue);
		UIContextStack->HandleEvent(Event);
	}
}

void ATBSPlayerController::MouseLeft()
{
	if (ClassLoader->GridUI->CursorOnGrid)
	{
		TBSUIContextCoordinateEvent Event = TBSUIContextCoordinateEvent(FName(TEXT("TileClick")), ClassLoader->GridUI->CurrentCoordinates);
		UIContextStack->HandleEvent(Event);
	}
}

void ATBSPlayerController::MouseRight()
{
	if (ClassLoader->GridUI->CursorOnGrid)
	{
		TBSUIContextCoordinateEvent Event = TBSUIContextCoordinateEvent(FName(TEXT("TileRightClick")), ClassLoader->GridUI->CurrentCoordinates);
		UIContextStack->HandleEvent(Event);
	}
}

void ATBSPlayerController::Escape()
{
	TBSUIContextEvent Event = TBSUIContextEvent(FName(TEXT("Escape")));
	UIContextStack->HandleEvent(Event);
}

void ATBSPlayerController::PropDebug()
{
	TBSUIContextEvent Event = TBSUIContextEvent(FName(TEXT("PropDebug")));
	UIContextStack->HandleEvent(Event);
}

void ATBSPlayerController::NewProp()
{
	if (Block)
	{
		Block->Debug = true;
	}

	//Server_CommandNewProp(ClassLoader->GridUI->CurrentCoordinates);
}

void ATBSPlayerController::Bomb()
{
	Server_CommandBomb(ClassLoader->GridUI->CurrentCoordinates);
}

void ATBSPlayerController::SendDebugMessage()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Sending debug message")));

	//std::string Message = "";

	//for (int32 i = 0; i < FMath::RandRange(1, 3); i++)
	//{		
	//	Message.append("0123456789");
	//}

	//TCPClient->Send((uint8_t*)Message.c_str(), Message.length());
	//TCPClient->Send((uint8_t*)Message.c_str(), Message.length());
	//TCPClient->Send((uint8_t*)Message.c_str(), Message.length());
}


void ATBSPlayerController::Server_CommandMoveUnit_Implementation(ATBSUnit* Unit, const TArray<FIntVector>& Movements)
{
	if (!HasAuthority())
	{
		return;		
	}

	ATBSGameMode* GameMode = GetWorld()->GetAuthGameMode<ATBSGameMode>();
	ATBSGameState* GameState = Cast<ATBSGameState>(GameMode->GameState);

	for (auto& Movement : Movements)
	{
		GameState->UnitManager->MoveUnit(Unit, Movement);
	}	
}

bool ATBSPlayerController::Server_CommandMoveUnit_Validate(ATBSUnit* Unit, const TArray<FIntVector>& Movements)
{
	if (PlayerNumber != Unit->PlayerNumber)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("This is not your unit!")));
		return false;
	}

	return true;
}

void ATBSPlayerController::Server_CommandBomb_Implementation(FIntVector Coordinates)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Bomb at (%i, %i, %i)"), Coordinates.X, Coordinates.Y, Coordinates.Z));
	
	ATBSGameMode* GameMode = GetWorld()->GetAuthGameMode<ATBSGameMode>();
	ATBSGameState* GameState = Cast<ATBSGameState>(GameMode->GameState);
	GameState->Bomb(Coordinates);
}

bool ATBSPlayerController::Server_CommandBomb_Validate(FIntVector Coordinates)
{
	return true;
}

void ATBSPlayerController::Server_CommandNewProp_Implementation(FIntVector Coordinates)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("New prop (%i, %i, %i)"), Coordinates.X, Coordinates.Y, Coordinates.Z));

	ATBSGameMode* GameMode = GetWorld()->GetAuthGameMode<ATBSGameMode>();
	ATBSGameState* GameState = Cast<ATBSGameState>(GameMode->GameState);
	GameState->SpawnNewProps(Coordinates);
}

bool ATBSPlayerController::Server_CommandNewProp_Validate(FIntVector Coordinates)
{
	return true;
}

void ATBSPlayerController::Server_ClientReady_Implementation()
{
	OnClientReady.Broadcast();
}

bool ATBSPlayerController::Server_ClientReady_Validate()
{
	return true;
}
