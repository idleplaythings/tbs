// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSGridPathFinder.h"
#include "TBSGridPathRenderer.h"
#include "TBSClassLoader.generated.h"

class ATBSPlayerController;
class ATBSDefaultPawn;
class ATBSGrid;
class ATBSGridUI;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClassesLoaded);

UCLASS()
class TBS_API ATBSClassLoader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBSClassLoader();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Initialise(ATBSPlayerController* InPlayerController);

	ATBSPlayerController* PlayerController;
	ATBSDefaultPawn* DefaultPawn;
	ATBSGrid* Grid;
	ATBSGridUI* GridUI;
	ATBSGridPathFinder* GridPathFinder;
	ATBSGridPathRenderer* GridPathRenderer;

	FOnClassesLoaded OnClassesLoaded;

private:
	FTimerHandle InitTimer;
	void TryLoadClasses();	
};
