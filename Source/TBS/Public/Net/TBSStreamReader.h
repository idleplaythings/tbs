// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class TBS_API TBSStreamReader
{
public:
	TBSStreamReader(uint32 HeaderLength);
	~TBSStreamReader();

	TArray<NetworkMessage> ReadData(char* Data, uint32 Length);

private:
	void ReadIntoMessageBuffer(char* Data, uint32 Length, TArray<NetworkMessage>& Messages);
	bool InitMessageBuffer(char* Data, uint32 Length);

	char* MessageBuffer = nullptr;
	char* PartialBuffer = nullptr;

	uint32 HeaderLength;
	uint32 DataOffset = 0;
	uint32 PartialLength = 0;
	uint32 PayloadLength = 0;
	uint32 BytesRead = 0;
};
