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
	InputComponent->BindAxis("AxisMoveCameraForward", this, &ATBSPlayerController::MoveCameraForward);
	InputComponent->BindAxis("AxisMoveCameraRight", this, &ATBSPlayerController::MoveCameraRight);

	// Testing stuff
	InputComponent->BindAction("ActionDebugMessage", IE_Pressed, this, &ATBSPlayerController::SendDebugMessage);
	InputComponent->BindAction("ActionNewProp", IE_Pressed, this, &ATBSPlayerController::NewProp);
	InputComponent->BindAction("ActionBomb", IE_Pressed, this, &ATBSPlayerController::Bomb);

	Server_ClientReady();

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Controller ready, server address %s"), *GetServerNetworkAddress()));

	//ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	//TSharedRef<FInternetAddr> Address = SocketSubsystem->CreateInternetAddr();
}

void ATBSPlayerController::Client_OpenSideChannelConnection_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Init side channel connection")));

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
	if (TCPClient->Connect(FString("192.168.0.107"), 10011))
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

void ATBSPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (ClassesLoaded)
	{
		FHitResult Result;
		GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, true, Result);
		ClassLoader->GridUI->HandleGridHitResult(Result);

		if (ClassLoader->GridUI->CoordinatesChanged)
		{
			UIContextStack->HandleEvent(new TBSUIContextCoordinateEvent(FName(TEXT("TileHoverEnd")), ClassLoader->GridUI->CurrentCoordinates));

			if (ClassLoader->GridUI->CursorOnGrid)
			{
				UIContextStack->HandleEvent(new TBSUIContextCoordinateEvent(FName(TEXT("TileHoverBegin")), ClassLoader->GridUI->CurrentCoordinates));
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

							ClassLoader->Grid->AddProp(Prop);
							int32 Rotation = (float)FMath::RandRange(0, 3) * 90;

							FIntVector BlockCell = FIntVector(
								FMath::FloorToInt(Prop.Coordinates.X / 300),
								FMath::FloorToInt(Prop.Coordinates.Y / 300),
								0
							);

							ATBSProp_Block** BlockPtr = BlockMap.Find(BlockCell);

							if (!BlockPtr)
							{
								Block = GetWorld()->SpawnActor<ATBSProp_Block>(ATBSProp_Block::StaticClass());
								Block->GameCoordinates = Prop.Coordinates;
								Block->Dimensions = FIntVector(1, 1, 1);
								Block->BlocksAccess = true;
								Block->Rotation = FRotator(0.0, Rotation, 0.0);
								Block->RecalculateCoordinates();
								BlockMap.Add(BlockCell, Block);
							}
							else
							{
								Block = *BlockPtr;
							}

							FCoordinateLocations Locations = ClassLoader->GridUI->GetCoordinateLocations(Prop.Coordinates);
							FTransform InstanceTransform(
								FRotator(0.0, Rotation, 0.0),
								Locations.Center,
								FVector((float)1 / 2, (float)1 / 2, (float)1 / 2)
							);

							Block->SpawnInstance(Prop.Coordinates, InstanceTransform);

							MessagePtr += sizeof(FProp);
						}
					}

					if (*(uint8_t*)MessagePtr == 0x02)
					{
						MessagePtr += 1;

						while (MessagePtr < Message.Data + Message.Length)
						{
							FIntVector Coordinates;
							memcpy(&Coordinates, MessagePtr + 1, sizeof(FIntVector));

							//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Received remove props at (%i, %i, %i)"), Coordinates.X, Coordinates.Y, Coordinates.Z));

							ClassLoader->Grid->RemovePropsAt(Coordinates);

							FIntVector BlockCell = FIntVector(
								FMath::FloorToInt(Coordinates.X / 300),
								FMath::FloorToInt(Coordinates.Y / 300),
								0
							);

							ATBSProp_Block** BlockPtr = BlockMap.Find(BlockCell);

							if (BlockPtr)
							{
								(*BlockPtr)->RemoveInstance(Coordinates);
							}

							MessagePtr += 1 + sizeof(FIntVector);
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
	UIContextStack->HandleEvent(new TBSUIContextEvent(FName(TEXT("ViewLevelUp"))));
}

void ATBSPlayerController::ViewLevelDown()
{
	UIContextStack->HandleEvent(new TBSUIContextEvent(FName(TEXT("ViewLevelDown"))));
}

void ATBSPlayerController::RotateCameraRight()
{
	UIContextStack->HandleEvent(new TBSUIContextEvent(FName(TEXT("RotateCameraRight"))));
}

void ATBSPlayerController::RotateCameraLeft()
{
	UIContextStack->HandleEvent(new TBSUIContextEvent(FName(TEXT("RotateCameraLeft"))));
}

void ATBSPlayerController::ZoomCameraIn()
{
	UIContextStack->HandleEvent(new TBSUIContextEvent(FName(TEXT("ZoomCameraIn"))));
}

void ATBSPlayerController::ZoomCameraOut()
{
	UIContextStack->HandleEvent(new TBSUIContextEvent(FName(TEXT("ZoomCameraOut"))));
}

void ATBSPlayerController::TogglePerspectiveCamera()
{
	UIContextStack->HandleEvent(new TBSUIContextEvent(FName(TEXT("TogglePerspectiveCamera"))));
}

void ATBSPlayerController::MoveCameraForward(float AxisValue)
{
	UIContextStack->HandleEvent(new TBSUIContextAxisEvent(FName(TEXT("MoveCameraForward")), AxisValue));
}

void ATBSPlayerController::MoveCameraRight(float AxisValue)
{
	UIContextStack->HandleEvent(new TBSUIContextAxisEvent(FName(TEXT("MoveCameraRight")), AxisValue));
}

void ATBSPlayerController::MoveCameraForwardOffset(float AxisValue)
{
	// This event is not triggered via the input component but by the HUD, so
	// ensure we have all the classes loaded before attempting to handle it
	if (ClassesLoaded)
	{
		UIContextStack->HandleEvent(new TBSUIContextAxisEvent(FName(TEXT("MoveCameraForwardOffset")), AxisValue));
	}
}

void ATBSPlayerController::MoveCameraRightOffset(float AxisValue)
{
	// This event is not triggered via the input component but by the HUD, so
	// ensure we have all the classes loaded before attempting to handle it
	if (ClassesLoaded)
	{
		UIContextStack->HandleEvent(new TBSUIContextAxisEvent(FName(TEXT("MoveCameraRightOffset")), AxisValue));
	}
}

void ATBSPlayerController::MouseLeft()
{
	if (ClassLoader->GridUI->CursorOnGrid)
	{
		UIContextStack->HandleEvent(new TBSUIContextCoordinateEvent(FName(TEXT("TileClick")), ClassLoader->GridUI->CurrentCoordinates));
	}
}

void ATBSPlayerController::MouseRight()
{
	if (ClassLoader->GridUI->CursorOnGrid)
	{
		UIContextStack->HandleEvent(new TBSUIContextCoordinateEvent(FName(TEXT("TileRightClick")), ClassLoader->GridUI->CurrentCoordinates));
	}
}

void ATBSPlayerController::Escape()
{
	UIContextStack->HandleEvent(new TBSUIContextEvent(FName(TEXT("Escape"))));
}

void ATBSPlayerController::NewProp()
{
	if (Block)
	{
		Block->Debug = true;
	}

	Server_CommandNewProp(ClassLoader->GridUI->CurrentCoordinates);
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
