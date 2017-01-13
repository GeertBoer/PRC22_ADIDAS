#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "encode.h"
#include "io.h"
#include "parity.h"

#define MAX_SUPPORTED_INPUT_SIZE 350


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

	 
	    uint8_t byteArray[MAX_SUPPORTED_INPUT_SIZE];
	    int amountOfBytes = 0;
	    getByteArrayFromFile(input, byteArray, 20, &amountOfBytes, 0);
	    if (amountOfBytes > MAX_SUPPORTED_INPUT_SIZE)
	    {
	    	printf("%s%d%s%d%s\n", "The chosen file is too big for this program (MAX ", MAX_SUPPORTED_INPUT_SIZE," BYTES, FILE WAS ", amountOfBytes, " BYTES BIG!)");
	    	return -1;
	    }

	    for (int i = 0; i < amountOfBytes; ++i)
	    {
	    	printf("Byte%d: %x\n", i, byteArray[i]);
	    }


	    int finalArraySize = 2 * amountOfBytes + 1;
		uint8_t finalArray[finalArraySize];
		finalArray[0] = 1;

		int counter = 1;
	    for(int i = 0; i < amountOfBytes; i++)
	    {
	    	uint8_t lsbParity = 0;
			uint8_t msbParity = 0;

			checkByte(byteArray[i], &lsbParity, &msbParity);

			uint8_t mergingArray[2];

			mergeNibblesAndParity(byteArray[i], lsbParity, msbParity, mergingArray);

			for (int q = 0; q <= 1; q++)
			{
				finalArray[counter] = mergingArray[q];
				counter++;
			}
	    }

	    writeByteArrayToFile(output, finalArray, &finalArraySize, mode);
	}
	else printf("\n########## %s ##\n", "Too few arguments!");
	
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