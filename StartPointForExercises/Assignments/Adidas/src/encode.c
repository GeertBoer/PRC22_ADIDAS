#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "encode.h"
#include "io.h"
#include "parity.h"

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

//FREAD PROTOTYPE:
//size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);


extern int 
encode(int argc, char * argv[])
{   
	if (argc >= 4)
	{
	    char input[20];
	    char output[20];

	    strcpy(input, argv[1]);
	    strcpy(output, argv[2]);
	    int mode = atoi(argv[3]);

	    printf("\nResults: %s %s %d\n\n", input, output, mode);
	    uint8_t byteArray[20];
	    getByteArrayFromFile(input, byteArray, 20);

		uint8_t lsbParity = 0;
		uint8_t msbParity = 0;

		checkByte(byteArray[0], &lsbParity, &msbParity);

		for (int i = 0; i < 20; i++)
		{
			printf("byteArray[%d]: 0x%x\n", i, byteArray[i]);
		}

		printf("\nLSB: 0x0%d\n", lsbParity);
		printf("MSB: 0x0%x\n", msbParity);

		uint8_t finalArray[2];
		mergeNibblesAndParity(byteArray[0], lsbParity, msbParity, finalArray);
	}
    return (0);
}

/*
int check(uint8_t incomingByte)
{
	uint8_t MSN =>> incomingByte;
	uint8_t LSB = (incomingByte =& 0x0F);

}
*/
/*
uint8_t check2(uint8_t incomingNibble)
{
	uint8_t incoming = incomingNibble;
	uint8_t tmp = (incomingNibble << 4);
	incoming |= tmp;

	uint8_t mask = 0x07;
	uint8_t amountOfSetBits = 0;
	for (int i = 1; i <= 4; i++)
	{
		amountOfSetBits += (incoming & mask);
		mask <<= 1;
	}

	return amountOfSetBits;
}
*/