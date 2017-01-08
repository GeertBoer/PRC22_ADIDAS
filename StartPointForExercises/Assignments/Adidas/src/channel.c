#include <stdio.h>
#include <stdlib.h>		// for srand()
#include <time.h>		// for time()
#include <stdint.h>
#include <math.h>

#include "channel.h"
#include "io.h"
#include "noise.h"

extern int 
channel(int argc, char * argv[])
{
	srand (time (NULL));	// take the current time as seed for random generator
	
	uint8_t byte = 0xFF;
	uint8_t bit = rand() % 8;

    addNoise(&byte, bit);
    
    return (0);
}