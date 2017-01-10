#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "parity.h"
#include "io.h"

#define MAX_SUPPORTED_INPUT_SIZE 50


uint8_t mergeOneByte(uint8_t nibble, uint8_t parity)
{
	uint8_t workingByte = 0;
	nibble <<= 3;
	workingByte |= nibble;
	workingByte |= parity;

	return workingByte;
}

void mergeNibblesAndParity(uint8_t byte, uint8_t lsbParity, uint8_t msbParity, uint8_t *arrayToWriteTo)
{
	uint8_t lsb = byte & 0x0F;
	uint8_t msb = (byte & 0xF0) >> 4;

	arrayToWriteTo[0] = mergeOneByte(msb, msbParity);
	arrayToWriteTo[1] = mergeOneByte(lsb, lsbParity);
}

uint8_t checkParityBit(uint8_t byte, uint8_t parityBit)
{
	uint8_t offset = 0;
	switch (parityBit)
	{
		case 0:
			offset = 0;
			break;
		case 1:
			offset = 3;
			break;
		case 2:
			offset = 1;
			break;
		default:
			return -1;
	}

	uint8_t hasParity = 0;

	uint8_t mask = 0x01;
	mask <<= offset;
	
	for (int i = 1; i <= 3; i++)
	{
		if (mask & byte)
		{
			hasParity++;
		}
		mask <<= 1;
	}

	return (hasParity & 0x01);
}

uint8_t checkNibble(uint8_t byte, uint8_t *parity)
{
	uint8_t byteToCheck = byte;
	byteToCheck |= (byte << 4);

	*parity = 0;

	for (int i = 0; i < 3; i++)
	{
		*parity <<= 1;
		*parity |= checkParityBit(byteToCheck, i);
	}

	return 0;
}

void checkByte(uint8_t byte, uint8_t *lsbParity, uint8_t *msbParity)
{
	uint8_t LSB = byte & 0x0F;
	uint8_t MSB = (byte & 0xF0) >> 4;
	uint8_t parityLSB = 0;
	uint8_t parityMSB = 0;

	checkNibble(MSB, &parityMSB);
	checkNibble(LSB, &parityLSB);

	*lsbParity = parityLSB;
	*msbParity = parityMSB;
}