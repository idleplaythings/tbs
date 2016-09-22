// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * Stream Reader
 *
 * Reads messages of arbitrary length from a binary stream that is fed to the stream
 * reader in chunks of arbitrary length. Each message begins with a 4 byte length header.
 */
class TBS_API TBSStreamReader
{
public:
	TBSStreamReader(uint32 HeaderLength);
	~TBSStreamReader();

	TArray<NetworkMessage> ReadData(uint8_t* Data, uint32 Length);

private:
	void ReadIntoMessageBuffer(uint8_t* InputBuffer, uint32 Length, TArray<NetworkMessage>& Messages);
	bool InitMessageBuffer(uint8_t* InputBuffer, uint32 Length);
	uint8_t* AllocateBuffer(uint8_t* Buffer, uint32 Length);

	uint8_t* MessageBuffer = nullptr;
	uint32 MessageLength = 0;

	uint8_t* PartialBuffer = nullptr;
	uint32 PartialLength = 0;

	uint32 HeaderLength;
	uint32 PayloadLength = 0;	
	uint32 PayloadBegin = 0; // Offset at which the actual payload in the buffer begins
	uint32 BytesRead = 0;
};
