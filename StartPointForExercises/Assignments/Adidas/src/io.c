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