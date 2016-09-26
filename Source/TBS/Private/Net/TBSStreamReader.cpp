// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSStreamReader.h"

TBSStreamReader::TBSStreamReader(uint32 HeaderLength) : HeaderLength(HeaderLength)
{
}

TBSStreamReader::~TBSStreamReader()
{
	FMemory::Free(MessageBuffer);
	FMemory::Free(PartialBuffer);
}

void TBSStreamReader::ReadData(uint8_t * Data, uint32 Length, TQueue<FNetworkMessage>& Messages)
{
	// Call ReadIntoMessageBuffer recursively until the binary data array has been exhausted
	ReadIntoMessageBuffer(Data, Length, Messages);

	// Fully received messages are returned as an array, partial message is retained in MessageBuffer
	//return Aaa;
}

void TBSStreamReader::ReadIntoMessageBuffer(uint8_t * InputBuffer, uint32 Length, TQueue<FNetworkMessage>& Messages)
{
	if (MessageLength == 0)
	{
		// Try and initialise the message buffer if we don't yet know the length of the message
		if (InitMessageBuffer(InputBuffer, Length))
		{
			PartialLength = 0;
		}
		else
		{
			return;
		}
	}

	// Read up to payload length of bytes from the buffer
	uint32 BytesToRead = PayloadLength - BytesRead;
	BytesToRead = BytesToRead > (Length - PayloadBegin) ? (Length - PayloadBegin) : BytesToRead;

	FMemory::Memcpy(MessageBuffer + BytesRead, InputBuffer + PayloadBegin, BytesToRead);

	BytesRead = BytesRead + BytesToRead;

	// If we received the full message, recursively call this function again to read the remainder of the buffer
	if (BytesRead == PayloadLength)
	{
		uint8_t* MessageData = (uint8_t*)FMemory::Malloc(PayloadLength);
		FMemory::Memcpy(MessageData, MessageBuffer, PayloadLength);

		Messages.Enqueue(FNetworkMessage(PayloadLength, MessageData));

		uint8_t* NewStart = InputBuffer + PayloadBegin + BytesToRead;
		uint32 NewLength = Length - PayloadBegin - BytesToRead;

		PartialLength = 0;
		PayloadLength = 0;
		BytesRead = 0;
		PayloadBegin = 0;
		MessageLength = 0;

		if (NewLength > 0)
		{
			ReadIntoMessageBuffer(NewStart, NewLength, Messages);
		}
	}
	else
	{
		PayloadBegin = 0;
	}	
}

/**
 * Initialises message buffer by parsing the HeaderLength sized header
 *
 * Since the incoming binary buffer is of an arbitrary length, it may not contain the full header,
 * or it may contain only the header and none of the actual data. In the "worst" case it may only
 * contain a single byte of the header.
 */
bool TBSStreamReader::InitMessageBuffer(uint8_t * InputBuffer, uint32 Length)
{
	// Case 1: We already have partially received the header
	if (PartialLength > 0)
	{
		// Case 1a: We have partial header, and this binary array contains the rest of the header
		if (PartialLength + Length > 4)
		{
			FMemory::Memcpy(PartialBuffer + PartialLength, InputBuffer, 4 - PartialLength);
			PayloadLength = *(reinterpret_cast<uint32 *>(PartialBuffer));
			MessageBuffer = AllocateBuffer(MessageBuffer, PayloadLength);
			MessageLength = PayloadLength;
			FMemory::Memcpy(MessageBuffer, InputBuffer + (4 - PartialLength), Length - (4 - PartialLength));
			BytesRead = 0;
			PayloadBegin = 4 - PartialLength;
			PartialLength = 0;
			return true;
		}

		// Case 1b: We have partial header, and this binary array contains another part,
		// but not the remainder of the header
		FMemory::Memcpy(PartialBuffer + PartialLength, InputBuffer, Length);
		PartialLength += Length;
		return false;
	}

	// Case 2: We haven't received any bytes of the header yet, and this buffer only contains a part of the header
	if (Length < 4)
	{
		PartialBuffer = AllocateBuffer(PartialBuffer, 4);
		//PartialBuffer = new uint8_t[4];
		FMemory::Memcpy(PartialBuffer, InputBuffer, Length);
		PartialLength = Length;

		return false;
	}

	// Case 3: We haven't received any bytes of the header yet, and this buffer contains the full header
	PayloadLength = *(reinterpret_cast<uint32 *>(InputBuffer));

	MessageBuffer = AllocateBuffer(MessageBuffer, PayloadLength);
	MessageLength = PayloadLength;
	BytesRead = 0;
	PayloadBegin = 4;
	return true;
}

uint8_t* TBSStreamReader::AllocateBuffer(uint8_t* Buffer, uint32 Length)
{
	if (Buffer != nullptr)
	{
		return (uint8_t*)FMemory::Realloc(Buffer, Length);
	}
	else
	{
		return (uint8_t*)FMemory::Malloc(Length);
	}
}
