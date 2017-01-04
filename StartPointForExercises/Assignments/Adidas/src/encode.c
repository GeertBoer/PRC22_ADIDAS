#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "encode.h"
#include "io.h"
#include "parity.h"


void getByteArrayFromFile(char *fileToRead, uint8_t *byteArray, int byteArraySize)
{
	FILE *fp;
    char filemode = 'r';
    fp = fopen(fileToRead, &filemode);

    fseek(fp, 0L, SEEK_SET);
    fseek(fp, 0L, SEEK_END);
    int sizeOfFile = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    int amountOfReadItems = fread(byteArray, 1, sizeOfFile, fp);
    int dummy = amountOfReadItems;
    amountOfReadItems = dummy;

    printf("DEC: %d\n", byteArray[0]);
    printf("HEX: %x\n\n", byteArray[0]);
    
    //return (amountOfReadItems);
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

uint8_t checkByte(uint8_t byte, uint8_t *lsbParity, uint8_t *msbParity)
{
	uint8_t LSB = byte & 0x0F;
	uint8_t MSB = (byte & 0xF0) >> 4;
	uint8_t parityLSB = 0;
	uint8_t parityMSB = 0;

	checkNibble(MSB, &parityMSB);
	checkNibble(LSB, &parityLSB);

	*lsbParity = parityLSB;
	*msbParity = parityMSB;

	return 0;
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

		uint8_t check = checkByte(byteArray[0], &lsbParity, &msbParity);

		uint8_t dummy = check;
		check = dummy;

		printf("LSB: 0x0%d\n", lsbParity);
		printf("MSB: 0x0%x\n", msbParity);
		
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