// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "TBSClassLoader.h"
#include "TBSUIContextStack.h"
#include "TBSGrid.h"
#include "TBSGridUI.h"
#include "TBSGridPathFinder.h"
#include "TBSGridPathRenderer.h"
#include "TBSPlayerState.h"
#include "TBSDefaultPawn.h"
#include "TBSTypes.h"
#include "TBSTCPClient.h"
#include "TBSProp_Block.h"
#include "TBSPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClientReady);

UCLASS()
class TBS_API ATBSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATBSPlayerController(const FObjectInitializer& ObjectInitializer);
	~ATBSPlayerController();

	virtual void PlayerTick(float DeltaSeconds) override;

	void EnableMouse();
	void BeginPlay();

	UPROPERTY(Replicated)
	int32 PlayerNumber = 0;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ClientReady();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_CommandMoveUnit(ATBSUnit* Unit, const TArray<FIntVector>& Movements);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_CommandBomb(FIntVector Coordinates);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_CommandNewProp(FIntVector Coordinates);

	//UFUNCTION(Client, Reliable)
	//void Client_CreateProps(TArray<FProp> const& PropArray);

	UFUNCTION(Client, Reliable)
	void Client_OpenSideChannelConnection();

	UFUNCTION()
	void OnClassesLoaded();

	ATBSUnit* SelectedUnit;
	TArray<FIntVector> SelectedPath;
	bool PathSelected = false;

	void ViewLevelUp();
	void ViewLevelDown();
	void RotateCameraRight();
	void RotateCameraLeft();
	void ZoomCameraIn();
	void ZoomCameraOut();
	void TogglePerspectiveCamera();
	void MoveCameraForward(float AxisValue);
	void MoveCameraRight(float AxisValue);
	void MouseLeft();
	void MouseRight();
	void Escape();
	void SendDebugMessage();
	void NewProp();
	void Bomb();
	void MoveCameraForwardOffset(float AxisValue);
	void MoveCameraRightOffset(float AxisValue);

	FOnClientReady OnClientReady;
	TBSTCPClient* TCPClient;

private:
	TBSUIContextStack* UIContextStack;
	ATBSClassLoader* ClassLoader;
	bool ClassesLoaded = false;

	int32 PropsReceived = 0;

	void TrySideChannelConnection();
	FTimerHandle SideChannelConnectionTimer;
	float SideChannelConnectionTimeout = 0.5;
	uint32 SideChannelConnectionAttempts = 10;


	TMap<FIntVector, ATBSProp_Block*> BlockMap;
	ATBSProp_Block* Block;
};

