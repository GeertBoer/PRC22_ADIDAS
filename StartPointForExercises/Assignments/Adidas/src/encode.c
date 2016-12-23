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

    printf("%d\n", byteArray[0]);
    
    //return (amountOfReadItems);
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
uint8_t checkParityBit(uint8_t byte, uint8_t parityBit)
{
	//0b00000110;
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

	uint8_t amount = 0;

	uint8_t mask = 0x01;
	mask <<= offset;
	
	for (int i = 1; i <= 3; i++)
	{
		if (mask & byte)
		{
			amount++;
		}
		mask <<= 1;
	}

	return (amount & 0x01);
}

uint8_t checkNibble(uint8_t byte, uint8_t *parityArray)
{
	uint8_t byteToSend = byte;
	byteToSend |= (byte << 4);

	for (int i = 0; i < 3; i++)
	{
		parityArray[i] = checkParityBit(byteToSend, i);
	}

	return 0;
}

uint8_t checkByte(uint8_t byte, uint8_t *lsbParity, uint8_t *msbParity)
{
	uint8_t LSB = byte & 0x0F;
	uint8_t MSB = (byte & 0xF0) >> 4;

	checkNibble(LSB, lsbParity);
	checkNibble(MSB, msbParity);

	return 0;
}

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

	    printf("\nresults: %s %s %d\n", input, output, mode);
	    uint8_t byteArray[20];
	    getByteArrayFromFile(input, byteArray, 20);


		uint8_t lsbParity[3];
		uint8_t msbParity[3];

		uint8_t check = checkByte(byteArray[0], lsbParity, msbParity);
		uint8_t dummy = check;
		check = dummy;

		printf("\n");

		for (int i = 0; i <= 2; i++)
		{
			printf("LSB: p%d: %d\n", i, lsbParity[i]);
		}

		printf("\n");

		for (int i = 0; i <= 2; i++)
		{
			printf("MSB: p%d: %d\n", i, msbParity[i]);
		}
	}
    return (0);
}