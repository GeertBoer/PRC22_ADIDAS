#include <stdio.h>
#include "io.h"
#include "resource_detector.h"

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
    int dummy = amountOfReadItems;
    amountOfReadItems = dummy;

    printf("DEC: %d\n", byteArray[0]);
    printf("HEX: %x\n\n", byteArray[0]);

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

void convertToMode2(uint8_t *inputArray, uint8_t *outputArray, int inputArraySize, int *outputArraySize)
{
	if (inputArray[0] == 1)
	{
		printf("This file IS MODE 1\n");
		uint8_t inputBit = 0;
		uint8_t outputBit = 7;
		int inputArrayPos = 1;
		int outputArrayPos = 1;

		while(inputArrayPos < inputArraySize)
		{
			outputArray[outputArrayPos] |= (((inputArray[inputArrayPos] & (1 << inputBit)) >> inputBit) << outputBit);
			if (inputBit >= 6)
			{
				inputBit = 0;
				inputArrayPos++;

			} else inputBit++;
			if (outputBit <= 0)
			{
				outputBit = 7;
				outputArrayPos++;
			} else outputBit--;
		} 

		printf("outputArrayPos: %d\n", outputArrayPos);
		printf("inputArrayPos: %d\n", inputArrayPos);

		outputArray[0] = 2;
		*outputArraySize = outputArrayPos;
	} else printf("This file IS NOT MODE 1\n");
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