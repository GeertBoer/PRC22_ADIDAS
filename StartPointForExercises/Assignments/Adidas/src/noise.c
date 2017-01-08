#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "noise.h"

int addNoise(uint8_t *byte, uint8_t bitToFlip)
{
	*byte ^= bitToFlip;

	return bitToFlip;
}