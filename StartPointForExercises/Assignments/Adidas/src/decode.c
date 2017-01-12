#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decode.h"
#include "io.h"
#include "parity.h"

#define MAX_SUPPORTED_INPUT_SIZE 50

extern int
decode (int argc, char * argv[])
{
    char input[20];
    char output[20];

    strcpy(input, argv[1]);
    strcpy(output, argv[2]);
    int mode = atoi(argv[3]);

    printf("\nResults: %s %s %d\n\n", input, output, mode);
 
    uint8_t byteArray[MAX_SUPPORTED_INPUT_SIZE];
    int amountOfBytes = 0;
    getByteArrayFromFile(input, byteArray, 20, &amountOfBytes);
    if (amountOfBytes > MAX_SUPPORTED_INPUT_SIZE)
    {
    	printf("%s%d%s%d%s\n", "The chosen file is too big for this program (MAX ", MAX_SUPPORTED_INPUT_SIZE," BYTES, FILE WAS ", amountOfBytes, " BYTES BIG!)");
    	return -1;
    }

    uint8_t newByteArray[amountOfBytes / 2];
    int counter = 0;
    for (int i = 1; i < amountOfBytes; i += 2)
    {
    	correctParity(&byteArray[i]);
    	correctParity(&byteArray[counter + 1]);
    	uint8_t MSN = ((byteArray[i] << 1) & 0xF0);
    	uint8_t LSN = ((byteArray[i + 1] >> 3) & 0x0F);
    	newByteArray[counter] = (MSN | LSN);
    	counter++;;
    }

    writeByteArrayToFile(output, byteArray, amountOfBytes);

    return (0);
}

