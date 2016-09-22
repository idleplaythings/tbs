// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Networking.h"
#include "TBSTCP.h"

/**
 * 
 */
class TBS_API TBSTCPClient : public TBSTCP
{
public:
	TBSTCPClient();
	~TBSTCPClient();

	virtual uint32 Run() override;

	bool Connect(FString IP, int32 Port);
	bool Send(uint8* Data, uint32 Length);

private:	
	FSocket* Socket;
	bool Connected = false;
};
