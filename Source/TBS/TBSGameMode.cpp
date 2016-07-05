// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSGameMode.h"

ATBSGameMode::ATBSGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetPawn();
	SetHUD();
	SetPlayerController();
}

void ATBSGameMode::SetPawn()
{
	static ConstructorHelpers::FClassFinder<APawn> TBSDefaultPawn(TEXT("/Game/Blueprints/BP_TBSDefaultPawn"));

	if (TBSDefaultPawn.Class != NULL)
	{
		DefaultPawnClass = TBSDefaultPawn.Class;
	}
}

void ATBSGameMode::SetHUD()
{
	static ConstructorHelpers::FClassFinder<AHUD> TBSHUD(TEXT("/Game/Blueprints/BP_TBSHUD"));

	if (TBSHUD.Class != NULL)
	{
		HUDClass = TBSHUD.Class;
	}
}

void ATBSGameMode::SetPlayerController()
{
	static ConstructorHelpers::FClassFinder<APlayerController> TBSPlayerController(TEXT("/Game/Blueprints/BP_TBSPlayerController"));

	if (TBSPlayerController.Class != NULL)
	{
		PlayerControllerClass = TBSPlayerController.Class;
	}
}