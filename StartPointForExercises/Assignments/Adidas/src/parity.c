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

uint8_t checkParityLSN(uint8_t byte, uint8_t parityBit)
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

	for (int i = 2; i >= 0; i--)
	{
		*parity <<= 1;
		*parity |= checkParityLSN(byteToCheck, i);
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

void reverseParity(uint8_t sourceByte, uint8_t *parity)
{
	*parity = 0;
	*parity |= (sourceByte & 0x07);
}

void correctParity(uint8_t *incomingByte)
{
    uint8_t parityOld = 0;
    uint8_t parityNew = 0;
    
    reverseParity(*incomingByte, &parityOld);
    checkNibble((*incomingByte >> 3), &parityNew);

    uint8_t comparedParity = parityOld ^ parityNew;

    int amountOfFailures = 0;
    for (int i = 0; i < 3; ++i)
    {
    	amountOfFailures += ((comparedParity >> i) & 1);
    }

    if (amountOfFailures == 1)
    {
    	*incomingByte &= 0xF8;
    	*incomingByte |= parityNew;
    }
    else
    {
	    switch (comparedParity)
	    {
	    	case 0x03:
	    		*incomingByte ^= 0x08;
	    		break;
	    	case 0x05:
	    		*incomingByte ^= 0x20;
	    		break;
	    	case 0x06:
	    		*incomingByte ^= 0x40;
	    		break;
	    	case 0x07:
	    		*incomingByte ^= 0x10;
	    		break;
	    }
	}
}


void correct2(uint8_t *incomingByte)
{
	uint8_t parityOld = 0;
    uint8_t parityNew = 0;
    
    reverseParity(*incomingByte, &parityOld);
    checkNibble((*incomingByte >> 3), &parityNew);

    uint8_t comparedParity = (parityOld ^ parityNew);
    uint8_t comparedParityIsOddOrEven = 0;
    for (int i = 0; i < 3; ++i)
    {
    	comparedParityIsOddOrEven ^= ((comparedParity >> i) & 1);
    }

    uint8_t allParityWrong = 1;
    for (int i = 0; i < 3; ++i)
    {
    	allParityWrong &= ((comparedParity >> i) & 1);
    }
    if(allParityWrong)
    {
    	*incomingByte ^= 4;
    }
    printf("comparedParityIsOddOrEven: %x\n", comparedParityIsOddOrEven);
    *incomingByte ^= (1 << (comparedParity - (comparedParityIsOddOrEven + (comparedParityIsOddOrEven * 2))));

    // *incomingByte ^= (1 << (comparedParity - comparedParityIsOddOrEven + (allParityWrong * 2)));
}