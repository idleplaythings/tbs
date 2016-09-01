// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSUIContextCoordinateEvent.h"

TBSUIContextCoordinateEvent::TBSUIContextCoordinateEvent()
{
}

TBSUIContextCoordinateEvent::TBSUIContextCoordinateEvent(FName Type, FIntVector Coordinates)
{
	this->Type = Type;
	this->Coordinates = Coordinates;

}

TBSUIContextCoordinateEvent::~TBSUIContextCoordinateEvent()
{
}
