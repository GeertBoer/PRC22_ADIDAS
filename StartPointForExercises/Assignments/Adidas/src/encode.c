#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "encode.h"
#include "io.h"
#include "parity.h"

extern int 
encode(int argc, char * argv[])
{
    // TODO
    /*
    char input[20];
    char output[20];

    strcpy(input, argv[1]);
    strcpy(output, argv[2]);
    int mode = atoi(argv[3]);

    printf("\nresults: %s %s %d\n", input, output, mode);

    FILE *fp;
    char filemode = 'r';
    fp = fopen(input, &filemode);

    uint8_t byteArray[20];

    fseek(fp, 0L, SEEK_SET);
    fseek(fp, 0L, SEEK_END);
    int sizeOfFile = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    int amountOfReadItems = fread(byteArray, 1, sizeOfFile, fp);

    printf("%d\n", byteArray[0]);
    
    return (amountOfReadItems);
    */

    return (0);
}

/*
int check(uint8_t incomingByte)
{
	uint8_t MSN =>> incomingByte;
	uint8_t LSB = (incomingByte =& 0x0F);

}
*/

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

//size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);