// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSTCP.h"

TBSTCP::TBSTCP()
{
}

TBSTCP::~TBSTCP()
{
}


bool TBSTCP::Init()
{
	Stopping = false;
	return true;
}

void TBSTCP::Stop()
{
	Stopping = true;
}

uint32 TBSTCP::Run()
{
	return 0;
}
