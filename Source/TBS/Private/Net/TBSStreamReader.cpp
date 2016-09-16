// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSStreamReader.h"

TBSStreamReader::TBSStreamReader(uint32 HeaderLength) : HeaderLength(HeaderLength)
{
}

TBSStreamReader::~TBSStreamReader()
{
}

TArray<NetworkMessage> TBSStreamReader::ReadData(char * Data, uint32 Length)
{
	TArray<NetworkMessage> Messages;

	ReadIntoMessageBuffer(Data, Length, Messages);

	return Messages;
}

void TBSStreamReader::ReadIntoMessageBuffer(char * Data, uint32 Length, TArray<NetworkMessage>& Messages)
{
	if (MessageBuffer == nullptr)
	{
		if (InitMessageBuffer(Data, Length))
		{
			delete PartialBuffer;
			PartialBuffer = nullptr;
			PartialLength = 0;
		}
		else
		{
			return;
		}
	}

	uint32 BytesToRead = PayloadLength - BytesRead;
	BytesToRead = BytesToRead > (Length - DataOffset) ? (Length - DataOffset) : BytesToRead;
	memcpy(MessageBuffer + BytesRead, Data + DataOffset, BytesToRead);

	BytesRead = BytesRead + BytesToRead;

	if (BytesRead == PayloadLength)
	{
		//char* Fixed = new char[PayloadLength + 1];
		//memcpy(Fixed, MessageBuffer, PayloadLength);
		//Fixed[PayloadLength] = '\0';

		char* MessageData = new char[PayloadLength];
		memcpy(MessageData, MessageBuffer, PayloadLength);

		Messages.Add(NetworkMessage(PayloadLength, MessageData));

		//NetworkMessageQueue.Enqueue(ANSI_TO_TCHAR(Fixed));

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Message of length %i read!"), BytesRead));

		delete MessageBuffer;
		MessageBuffer = nullptr;
		delete PartialBuffer;
		PartialBuffer = nullptr;

		char* NewStart = Data + DataOffset + BytesToRead;
		uint32 NewLength = Length - DataOffset - BytesToRead;

		PartialLength = 0;
		PayloadLength = 0;
		BytesRead = 0;
		DataOffset = 0;

		if (NewLength > 0)
		{
			ReadIntoMessageBuffer(NewStart, NewLength, Messages);
		}
	}

	DataOffset = 0;
}

bool TBSStreamReader::InitMessageBuffer(char * Data, uint32 Length)
{
	if (PartialLength > 0)
	{
		if (PartialLength + Length > 4)
		{
			memcpy(PartialBuffer + PartialLength, Data, 4 - PartialLength);
			PayloadLength = *(reinterpret_cast<uint32 *>(PartialBuffer));
			MessageBuffer = new char[PayloadLength];
			memcpy(MessageBuffer, Data + (4 - PartialLength), Length - (4 - PartialLength));
			BytesRead = 0;
			DataOffset = 4 - PartialLength;

			delete PartialBuffer;
			PartialBuffer = nullptr;
			PartialLength = 0;
			return true;
		}

		memcpy(PartialBuffer + PartialLength, Data, Length);
		PartialLength += Length;
		return false;
	}

	if (Length < 4)
	{
		PartialBuffer = new char[4];
		memcpy(PartialBuffer, Data, Length);
		PartialLength = Length;

		return false;
	}

	PayloadLength = *(reinterpret_cast<uint32 *>(Data));

	MessageBuffer = new char[PayloadLength];
	BytesRead = 0;
	DataOffset = 4;
	return true;
}
