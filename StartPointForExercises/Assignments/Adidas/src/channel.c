#include <stdio.h>
#include <stdlib.h>		// for srand()
#include <time.h>		// for time()

#include "channel.h"
#include "io.h"
#include "noise.h"

extern int 
channel(int argc, char * argv[])
{
	srand (time (NULL));	// take the current time as seed for random generator
	
    // TODO
    
    return (0);
}
