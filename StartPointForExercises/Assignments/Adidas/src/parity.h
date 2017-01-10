#ifndef _PARITY_H
#define _PARITY_H

#include <stdint.h>
#include "parity.h"

#define MAX_SUPPORTED_INPUT_SIZE 50

uint8_t mergeOneByte(uint8_t nibble, uint8_t parity);

void mergeNibblesAndParity(uint8_t byte, uint8_t lsbParity, uint8_t msbParity, uint8_t *arrayToWriteTo);

uint8_t checkParityBit(uint8_t byte, uint8_t parityBit);

uint8_t checkNibble(uint8_t byte, uint8_t *parity);

void checkByte(uint8_t byte, uint8_t *lsbParity, uint8_t *msbParity);

#endif