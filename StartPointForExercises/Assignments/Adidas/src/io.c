#include <stdio.h>
#include "io.h"
#include "resource_detector.h"

#define MAX_SUPPORTED_INPUT_SIZE 50

void convertToMode(uint8_t *arrayToConvert, int *arraySize, int modeToConvertTo)
{
	if (arrayToConvert[0] != modeToConvertTo)
	{
		int inputArraySize = *arraySize;

		printf("This file IS MODE %d\n", modeToConvertTo);
		uint8_t inputBit = 0;
		uint8_t outputBit = 0;
		int inputArrayPos = 1;
		int outputArrayPos = 1;

		uint8_t tmpArray[inputArraySize];
		for (int i = 0; i < inputArraySize; ++i)
		{
			tmpArray[i] = arrayToConvert[i];
			arrayToConvert[i] = 0;
		}

		if (modeToConvertTo == 2)
		{
			printf("Convering to Mode 2\n");
			inputBit = 6;
			outputBit = 7;
			while(inputArrayPos < inputArraySize)
			{
				arrayToConvert[outputArrayPos] |= (((tmpArray[inputArrayPos] & (1 << inputBit)) >> inputBit) << outputBit);
				if (inputBit <= 0)
				{
					inputBit = 6;
					inputArrayPos++;
				} else inputBit--;
				if (outputBit <= 0)
				{
					outputBit = 7;
					outputArrayPos++;
				} else outputBit--;
			}
			arrayToConvert[0] = modeToConvertTo; 
		}
		else if (modeToConvertTo == 1)
		{
			printf("Converting to Mode 1\n");
			inputBit = 7;
			outputBit = 0;
			while(inputArrayPos < inputArraySize)
			{
				arrayToConvert[outputArrayPos] |= (((tmpArray[inputArrayPos] & (1 << inputBit)) >> inputBit) << outputBit);
				if (inputBit <= 0)
				{
					inputBit = 7;
					inputArrayPos++;

				} else inputBit--;
				if (outputBit >= 6)
				{
					outputBit = 6;
					outputArrayPos++;
				} else outputBit++;
			}
			arrayToConvert[0] = modeToConvertTo;
		}

		printf("outputArrayPos: %d\n", outputArrayPos);
		printf("inputArrayPos: %d\n", inputArrayPos);

		*arraySize = (outputArrayPos + 1);
	} else printf("This file IS MODE %d, modeToConvertTo = %d\n", arrayToConvert[0], modeToConvertTo);
}

void getByteArrayFromFile(char *fileToRead, uint8_t *byteArray, int byteArraySize, int *amountOfBytesInFile)
{
	FILE *fp;
    char filemode = 'r';
    fp = fopen(fileToRead, &filemode);


    fseek(fp, 0L, SEEK_SET);
    fseek(fp, 0L, SEEK_END);
    int sizeOfFile = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    int amountOfReadItems = fread(byteArray, 1, sizeOfFile, fp);

    if(sizeOfFile != amountOfReadItems)
    {
    	printf("NOT ENOUGH ITEMS READ\n");
    }

    *amountOfBytesInFile = sizeOfFile;

    fclose(fp);
}

void writeByteArrayToFile(char *fileToWrite, uint8_t *byteArray, int byteArraySize)
{
	FILE *fp;
	char filemode = 'w';
	fp = fopen(fileToWrite, &filemode);

	fwrite(byteArray, 1, byteArraySize, fp);

	fclose(fp);
}



void convertToMode1(uint8_t *inputArray, uint8_t *outputArray, int inputArraySize, int *outputArraySize)
{ 
	printf("inputArray[0]%d\n", inputArray[0]);
	if (inputArray[0] == 2)
	{
		printf("This file IS MODE 2\n");

		uint8_t inputBit = 7;
		uint8_t outputBit = 0;
		int inputArrayPos = 1;
		int outputArrayPos = 1;

		while(inputArrayPos < inputArraySize)
		{
			outputArray[outputArrayPos] |= (((inputArray[inputArrayPos] & (1 << inputBit)) >> inputBit) << outputBit);
			if (inputBit <= 0)
			{
				inputBit = 7;
				inputArrayPos++;

			} else inputBit--;
			if (outputBit >= 6)
			{
				outputBit = 0;
				outputArrayPos++;
			} else outputBit++;
		}

		printf("outputArrayPos: %d\n", outputArrayPos);
		printf("inputArrayPos: %d\n", inputArrayPos);

		outputArray[0] = 1;
		*outputArraySize = outputArrayPos;
	}
	else printf("This file IS NOT MODE 2\n");
}