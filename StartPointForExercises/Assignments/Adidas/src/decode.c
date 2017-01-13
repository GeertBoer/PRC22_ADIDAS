#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decode.h"
#include "io.h"
#include "parity.h"

#define MAX_SUPPORTED_INPUT_SIZE 73

extern int
decode (int argc, char * argv[])
{


	/*  DIT WERKT!
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
    int amountOfBytesInNewByteArray = 0;
    for (int i = 1; i < amountOfBytes; i += 2)
    {
    	correct2(&byteArray[i]);
    	correct2(&byteArray[i + 1]);
    	uint8_t MSN = ((byteArray[i] << 1) & 0xF0);
    	uint8_t LSN = ((byteArray[i + 1] >> 3) & 0x0F);
    	newByteArray[amountOfBytesInNewByteArray] = (MSN | LSN);
    	amountOfBytesInNewByteArray++;
    }

    writeByteArrayToFile(output, newByteArray, amountOfBytesInNewByteArray);
*/
    return (0);
}



/*  MODE SWITCH TEST CODE
	int arraysize = 100;
	uint8_t dummyArray[arraysize];
	uint8_t tmpArray[arraysize];
	uint8_t outarray[arraysize];
	for (int i = 0; i < arraysize; ++i)
	{
		dummyArray[i] = 0x03;
		tmpArray[i] = 0;
		outarray[i] = 0;
	}
	dummyArray[0] = 1;

	int tmpArraySize = 0;
	int outputArraySize = 0;

	convertToMode2(dummyArray, tmpArray, arraysize, &tmpArraySize);
	convertToMode1(tmpArray, outarray, arraysize, &outputArraySize);

	for (int i = 0; i < arraysize; ++i)
	{
		printf("%d: %x\n", i, outarray[i]);
	}

	char output1[20];
	strcpy(output1, argv[1]);
	writeByteArrayToFile(output1, dummyArray, arraysize);

	char output2[20];
	strcpy(output2, argv[2]);
	writeByteArrayToFile(output2, tmpArray, tmpArraySize);

	char output3[20];
	strcpy(output3, argv[3]);
	writeByteArrayToFile(output3, outarray, outputArraySize);
	*/