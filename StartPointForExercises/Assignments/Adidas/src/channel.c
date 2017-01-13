#include <stdio.h>
#include <stdlib.h>		// for srand()
#include <time.h>		// for time()
#include <stdint.h>
#include <string.h>

#include "channel.h"
#include "io.h"
#include "noise.h"

#define MAX_SUPPORTED_INPUT_SIZE 700

extern int 
channel(int argc, char * argv[])
{
	srand (time (NULL));	// take the current time as seed for random generator

	if (argc >= 3)
	{
		char input[20];
		strcpy(input, argv[1]);
		char output[20];
		strcpy(output, argv[2]);

		uint8_t byteArray[MAX_SUPPORTED_INPUT_SIZE];
		int amountOfBytesInArray = 0;


		getByteArrayFromFile(input, byteArray, MAX_SUPPORTED_INPUT_SIZE, &amountOfBytesInArray, 0);
		printf("amountOfBytesInArray: %d    MAX_SUPPORTED_INPUT_SIZE: %d\n", amountOfBytesInArray, MAX_SUPPORTED_INPUT_SIZE);

		for(int i = 0; i <= amountOfBytesInArray; i += 2)
		{
			addNoise(&byteArray[i]);
		}

		writeByteArrayToFile(output, byteArray, &amountOfBytesInArray, 0);
	}

    return (0);
}