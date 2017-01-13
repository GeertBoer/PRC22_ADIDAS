#ifndef _IO_H
#define _IO_H

#include "io.h"
#include "stdint.h"

void getByteArrayFromFile(char *fileToRead, uint8_t *byteArray, int byteArraySize, int *amountOfBytesInFile);
void writeByteArrayToFile(char *fileToWrite, uint8_t *byteArray, int byteArraySize);
void convertToMode(uint8_t *arrayToConvert, int *arraySize, int modeToConvertTo);
void convertToMode1(uint8_t *inputArray, uint8_t *outputArray, int inputArraySize, int *outputArraySize);

#endif