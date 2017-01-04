#ifndef _IO_H
#define _IO_H

#include "io.h"
#include "stdint.h"

void getByteArrayFromFile(char *fileToRead, uint8_t *byteArray, int byteArraySize, int *amountOfBytesInFile);

#endif