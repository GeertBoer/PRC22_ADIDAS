#ifndef _PARITY_H
#define _PARITY_H

#include <stdint.h>
#include "parity.h"

#define MAX_SUPPORTED_INPUT_SIZE 50

void mergeNibblesAndParity(uint8_t byte, uint8_t lsbParity, uint8_t msbParity, uint8_t *arrayToWriteTo);

void checkByte(uint8_t byte, uint8_t *lsbParity, uint8_t *msbParity);

void reverseParity(uint8_t sourceByte, uint8_t *parity);

uint8_t checkNibble(uint8_t byte, uint8_t *parity);

void correctParity(uint8_t *incomingByte);

#endif