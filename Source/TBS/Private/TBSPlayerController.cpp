// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "UnrealNetwork.h"
#include "TBSPlayerState.h"
#include "TBSGameMode.h"
#include "TBSGameState.h"
#include "TBSUIDefaultContext.h"
#include "TBSUIContextAxisEvent.h"
#include "TBSUIContextCoordinateEvent.h"
#include "TBSPlayerController.h"

ATBSPlayerController::ATBSPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	EnableMouse();
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

	//ClassLoader->Grid->ReindexProps();
	Server_ClientReady();
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

void ATBSPlayerController::Server_HandleCommand_Implementation(ATBSUnit* Unit, const TArray<FIntVector>& Movements)
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

bool ATBSPlayerController::Server_HandleCommand_Validate(ATBSUnit* Unit, const TArray<FIntVector>& Movements)
{
	if (PlayerNumber != Unit->PlayerNumber)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("This is not your unit!")));
		return false;
	}

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

void ATBSPlayerController::Client_CreateProps_Implementation(TArray<FProp> const& PropArray)
{
	if (Role < ROLE_Authority)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Received props %i"), PropArray.Num()));

		for (auto& Prop : PropArray)
		{
			int32 Rotation = (float) FMath::RandRange(0, 3) * 90;
			ATBSProp* PropActor = ClassLoader->PropFactory->CreateBlock(Prop.Coordinates, FIntVector(1, 1, 6), FRotator(0.0, Rotation, 0.0));
			ClassLoader->Grid->AddProp(PropActor);
			ClassLoader->PropManager->ResetProp(PropActor);
		}		

		PropsReceived += PropArray.Num();

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Total props received %i"), PropsReceived));
	}		
}
