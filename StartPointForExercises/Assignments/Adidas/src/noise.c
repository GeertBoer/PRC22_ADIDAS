#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "noise.h"
#include "encode.h"

void checkNoise(uint8_t old, uint8_t new)
{
	uint8_t result = (old ^ new);
	uint8_t bit = 0;

	printf("\n####LSB\n");
	for (int i = 7; i >= 0; i--)
	{
		printf("Bit %d is ", bit);
		if (result & (1 << i))
		{
			printf("flipped\n");
		}
		else printf("NOT flipped\n");
		bit++;
	}

	printf("####MSB\n");
}

void addNoise(uint8_t *byte)
{
	uint8_t bitToFlip = rand() % 8;
	uint8_t mask = 0x01;
	uint8_t oldByte = *byte;
	mask <<= bitToFlip;
	*byte ^= mask;

	checkNoise(oldByte, *byte);
}